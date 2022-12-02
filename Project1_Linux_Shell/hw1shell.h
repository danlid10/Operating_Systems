#ifndef HW1_H
#define HW1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_LINE 1024 // max characters in line
#define MAX_ARGS 64	  // max arguments in line (incluiding command)
#define MAX_BG_PS 4	  // max running background processes

typedef struct Process
{
	char input[MAX_LINE];
	int pid;
	int is_background;
	struct Process *next;
} Process;

Process *create_process_node();
Process *add_process_node(Process *processes, Process *new_process);
Process *remove_process_node(Process *processes, int pid);
int process_list_len(Process *processes);
void display_jobs(Process *processes);
void split_line_by_spaces(char line[], char *args[]);
void parse_input(char line[], char *cmd_args[], Process *myproc);
Process *zombie_killer(Process *processes);
Process *execute_command(char line[], Process *processes);

#endif