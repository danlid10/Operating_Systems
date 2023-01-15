#ifndef HW5_H
#define HW5_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "fs.h"

void free_structs(struct superblock *superblock, struct dinode *dinode, struct dirent *dirent);
struct superblock *read_superblock(FILE *fp);
struct dinode *read_dinodes(FILE *fp, uint inodestart, uint ninodes);
struct dirent *read_dirents(FILE *fp, struct dinode *inode);
int num_digits(int num);
int find_inum_by_name(struct dirent *dirs, char name[]);
void ls(struct dirent *dirs, int inum, struct dinode *inodes, uint ninodes);
void cp_xv6_linux(FILE *fp_xv6_img, FILE *fp_linuxfile, struct dinode *inode);

#define MIN(a, b) (a < b ? a : b)

#endif