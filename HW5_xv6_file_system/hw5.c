#include "hw5.h"

int main(int argv, char *argc[])
{
    int inum;
    FILE *fp_xv6_img = NULL, *fp_linuxfile = NULL;
    struct superblock *superblock = NULL;
    struct dinode *inodes = NULL;
    struct dirent *root_dirs = NULL;

    if (argv < 3)
    {
        printf("[ERROR] Invalid command line arguments\n");
        return 1;
    }

    fp_xv6_img = fopen(argc[1], "rb");
    if (fp_xv6_img == NULL)
    {
        printf("[ERROR] '%s': %s\n", argc[1], strerror(errno));
        return 1;
    }

    superblock = read_superblock(fp_xv6_img);

    inodes = read_dinodes(fp_xv6_img, superblock->inodestart, superblock->ninodes);

    root_dirs = read_dirents(fp_xv6_img, &inodes[ROOTINO]);

    if (!strcmp(argc[2], "ls")) // ls
    {
        ls(root_dirs, ROOTINO, inodes, superblock->ninodes);
    }
    else if (!strcmp(argc[2], "cp")) // cp [xv6file] [linuxfile]
    {
        if (argv < 5)
        {
            printf("[ERROR] Invalid command line arguments\n");
            fclose(fp_xv6_img);
            free_structs(superblock, inodes, root_dirs);
            return 1;
        }
        inum = find_inum_by_name(root_dirs, argc[3]);
        if (inum == -1) // xv6file not found
            printf("[ERROR] File '%s' does not exist in the root directory of '%s'\n", argc[3], argc[1]);
        else
        {
            fp_linuxfile = fopen(argc[4], "wb");
            if (fp_linuxfile == NULL)
            {
                printf("[ERROR] '%s': %s\n", argc[4], strerror(errno));
                fclose(fp_xv6_img);
                free_structs(superblock, inodes, root_dirs);
                return 1;
            }
            cp_xv6_linux(fp_xv6_img, fp_linuxfile, &inodes[inum]);
            fclose(fp_linuxfile);
        }
    }
    else
    {
        printf("[ERROR] Invalid command line arguments\n");
        fclose(fp_xv6_img);
        free_structs(superblock, inodes, root_dirs);
        return 1;
    }

    fclose(fp_xv6_img);
    free_structs(superblock, inodes, root_dirs);

    return 0;
}

void free_structs(struct superblock *superblock, struct dinode *dinode, struct dirent *dirent)
{
    if (superblock != NULL)
        free(superblock);
    if (dinode != NULL)
        free(dinode);
    if (dirent != NULL)
        free(dirent);
}
