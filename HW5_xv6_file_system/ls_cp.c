#include "hw5.h"

// Return the number of digits of given number.
int num_digits(int num)
{
    int count = 0;
    do
    {
        num /= 10;
        count++;
    } while (num != 0);

    return count;
}

// Find the inode number of an inode by its name in directory (dirent array).
// Return the inode number. Return -1 if not found.
int find_inum_by_name(struct dirent *dirs, char name[])
{
    int i;

    for (i = 0; i < DPF; i++)
    {
        if (dirs[i].inum == 0)
            continue;

        if (!strcmp(dirs[i].name, name))
            return dirs[i].inum;
    }

    return -1;
}

// Print 'ls' of a directory (dirent array).
void ls(struct dirent *dirs, int inum, struct dinode *inodes, uint ninodes)
{
    int i;

    for (i = 0; i < inodes[inum].size / sizeof(struct dirent); i++)
    {
        if (dirs[i].inum == 0)
            continue;

        printf("%-*s ", DIRSIZ - 1, dirs[i].name);
        printf("%d ", inodes[dirs[i].inum].type);
        printf("%*d  ", num_digits(ninodes), dirs[i].inum);
        printf("%*u\n", num_digits(MAXFILE * BSIZE), inodes[dirs[i].inum].size);
    }
}

// Extract the contents of the file form the given inode and write it to a linux file using the given file pointer.
void cp_xv6_linux(FILE *fp_xv6_img, FILE *fp_linuxfile, struct dinode *inode)
{
    uint i, nblocks, remaining_bytes;
    char buffer[BSIZE];
    uint indirect_addrs[NINDIRECT], last_block_addrs;

    nblocks = inode->size / BSIZE;

    // Direct blocks
    for (i = 0; i < MIN(NDIRECT, nblocks); i++)
    {
        fseek(fp_xv6_img, inode->addrs[i] * BSIZE, SEEK_SET);
        fread(buffer, 1, BSIZE, fp_xv6_img);
        fwrite(buffer, 1, BSIZE, fp_linuxfile);
    }
    last_block_addrs = inode->addrs[i];

    // Indirect blocks
    if (nblocks > NDIRECT)
    {
        // Read indirect blocks addresses
        fseek(fp_xv6_img, inode->addrs[NDIRECT] * BSIZE, SEEK_SET);
        fread(indirect_addrs, NINDIRECT, sizeof(uint), fp_xv6_img);

        for (i = NDIRECT; i < MIN(MAXFILE, nblocks); i++)
        {
            fseek(fp_xv6_img, indirect_addrs[i - NDIRECT] * BSIZE, SEEK_SET);
            fread(buffer, 1, BSIZE, fp_xv6_img);
            fwrite(buffer, 1, BSIZE, fp_linuxfile);
        }
        last_block_addrs = indirect_addrs[i - NDIRECT];
    }

    // Data from incomplete block at the end
    remaining_bytes = inode->size % BSIZE;
    if (remaining_bytes != 0)
    {
        fseek(fp_xv6_img, last_block_addrs * BSIZE, SEEK_SET);
        fread(buffer, 1, remaining_bytes, fp_xv6_img);
        fwrite(buffer, 1, remaining_bytes, fp_linuxfile);
    }
}