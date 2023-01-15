#include "hw5.h"

// Read superblock from xv6 img file (fs.img).
struct superblock *read_superblock(FILE *fp)
{
    struct superblock *superblock = NULL;

    superblock = (struct superblock *)malloc(sizeof(struct superblock));
    if (superblock == NULL)
    {
        printf("[ERROR] malloc failed\n");
        exit(1);
    }
    fseek(fp, BSIZE, SEEK_SET);
    fread(superblock, 1, sizeof(struct superblock), fp);

    return superblock;
}

// Read all inodes from xv6 img file (fs.img).
struct dinode *read_dinodes(FILE *fp, uint inodestart, uint ninodes)
{
    struct dinode *inodes = NULL;

    // Array of {ninodes * sizeof(struct dinode)} structs dinode
    inodes = (struct dinode *)malloc(ninodes * sizeof(struct dinode));
    if (inodes == NULL)
    {
        printf("[ERROR] malloc failed\n");
        exit(1);
    }

    fseek(fp, inodestart * BSIZE, SEEK_SET);
    fread(inodes, ninodes, sizeof(struct dinode), fp);

    return inodes;
}

/* Read array of dirent structs from a dinode struct.
Return allocated array of dirent structs.
if inode type is not a directory (1), return NULL. */
struct dirent *read_dirents(FILE *fp, struct dinode *inode)
{
    uint i, ndirents, nblocks, remaining_dirents;
    struct dirent *dirs = NULL;
    uint indirect_addrs[NINDIRECT], last_block_addrs;

    if (inode->type != 1) // Not a directory
        return NULL;

    ndirents = inode->size / sizeof(struct dirent);
    nblocks = inode->size / BSIZE;

    // Array of ndirents structs dirent
    dirs = (struct dirent *)malloc(ndirents * sizeof(struct dirent));
    if (dirs == NULL)
    {
        printf("[ERROR] malloc failed\n");
        exit(1);
    }

    // Direct blocks
    for (i = 0; i < MIN(NDIRECT, nblocks); i++)
    {
        fseek(fp, inode->addrs[i] * BSIZE, SEEK_SET);
        fread(&dirs[i * DPB], DPB, sizeof(struct dirent), fp);
    }
    last_block_addrs = inode->addrs[i];

    // Indirect blocks
    if (nblocks > NDIRECT)
    {

        // Read indirect blocks addresses
        fseek(fp, inode->addrs[NDIRECT] * BSIZE, SEEK_SET);
        fread(indirect_addrs, NINDIRECT, sizeof(uint), fp);

        for (i = NDIRECT; i < MIN(MAXFILE, nblocks); i++)
        {
            fseek(fp, indirect_addrs[i - NDIRECT] * BSIZE, SEEK_SET);
            fread(&dirs[i * DPB], DPB, sizeof(struct dirent), fp);
        }
        last_block_addrs = indirect_addrs[i - NDIRECT];
    }

    // Data from incomplete block at the end
    remaining_dirents = (inode->size % BSIZE) / sizeof(struct dirent);
    if (remaining_dirents != 0)
    {
        fseek(fp, last_block_addrs * BSIZE, SEEK_SET);
        fread(&dirs[i * DPB], remaining_dirents, sizeof(struct dirent), fp);
    }

    return dirs;
}
