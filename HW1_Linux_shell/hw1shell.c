#include "hw1shell.h"

Process *create_process_node()
{
	Process *new_process = (Process *)malloc(sizeof(Process));
	if (new_process == NULL)
		printf("hw1shell: malloc failed\n");
	else
		new_process->next = NULL;

	return new_process;
}

Process *add_process_node(Process *processes, Process *new_process)
{
	new_process->next = processes;
	return new_process;
}

int process_list_len(Process *processes)
{
	Process *curr = processes;
	int len = 0;
	while (curr != NULL)
	{
		len++;
		curr = curr->next;
	}
	return len;
}

Process *remove_process_node(Process *processes, int pid)
{
	Process *temp;
	Process *curr = processes;
	if (processes->pid == pid)
	{
		temp = processes;
		processes = processes->next;
		free(temp);
	}
	else
	{
		while (curr->next != NULL)
		{
			if (curr->next->pid == pid)
			{
				temp = curr->next;
				curr->next = curr->next->next;
				free(temp);
				break;
			}
			else
				curr = curr->next;
		}
	}
	return processes;
}

void display_jobs(Process *processes)
{
	Process *curr = processes;
	while (curr != NULL)
	{
		printf("%d %s", curr->pid, curr->input);
		curr = curr->next;
	}
}

void split_line_by_spaces(char line[], char *args[])
{
	char *token;
	int arg_count = 0;

	token = strtok(line, " \t\n\r");
	while (token != NULL)
	{
		args[arg_count] = token;
		arg_count++;
		token = strtok(NULL, " \t\n\r");
	}

	args[arg_count] = NULL;
}

void parse_input(char line[], char *cmd_args[], Process *new_proc)
{
	int amp_loc, is_bg_ps;

	strncpy(new_proc->input, line, MAX_LINE);
	amp_loc = strcspn(line, "&");	   // offset of first &
	is_bg_ps = amp_loc < strlen(line); // background process
	if (is_bg_ps)
		line[amp_loc] = '\0'; // remove & and text after it
	split_line_by_spaces(line, cmd_args);
	new_proc->is_background = is_bg_ps;
}

Process *zombie_killer(Process *processes)
{
	int pid, status;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
		{
			printf("hw1shell: pid %d finished\n", pid);
			processes = remove_process_node(processes, pid);
		}

	return processes;
}

Process *execute_command(char line[], Process *processes)
{
	char *cmd_args[MAX_ARGS];
	int pid, status;
	Process *new_proc;

	new_proc = create_process_node();
	if (new_proc == NULL) // malloc failed
	{
		while (process_list_len(processes) != 0)
			processes = zombie_killer(processes);
		exit(1);
	}

	parse_input(line, cmd_args, new_proc);

	if (cmd_args[0] != NULL) // not empty command
	{
		if (!strcmp(cmd_args[0], "cd")) // Internal command: cd
		{
			if (cmd_args[2] != NULL)
				printf("hw1shell: invalid command\n");
			else
				status = chdir(cmd_args[1]);
			if (status == -1)
				printf("hw1shell: invalid command\n");
		}
		else if (!strcmp(cmd_args[0], "jobs")) // Internal command: jobs
			display_jobs(processes);
		else if (!strcmp(cmd_args[0], "exit")) // Internal command: exit
		{
			while (process_list_len(processes) != 0)
				processes = zombie_killer(processes);
			exit(0);
		}
		else // external commands
		{
			if (process_list_len(processes) == MAX_BG_PS && new_proc->is_background) // check for max background processes
				printf("hw1shell: too many background commands running\n");
			else //  foreground process or enough space for another backround process
			{
				fflush(stdout);
				pid = fork();
				if (pid > 0) // parent process
				{
					new_proc->pid = pid;
					if (!new_proc->is_background) // foregroud process
						waitpid(pid, &status, 0);
					else // backgroud process
					{
						processes = add_process_node(processes, new_proc);
						printf("hw1shell: pid %d started\n", pid);
					}
				}
				else if (pid == 0) // child process
				{
					status = execvp(cmd_args[0], cmd_args);
					if (status == -1)
						{
							printf("hw1shell: invalid command\n");
							printf("hw1shell: execvp failed, errno is %d\n", errno);
						}
					exit(0);
				}
				else // Fork failed
					printf("hw1shell: fork failed, errno is %d\n", errno);
			}
		}
	}
	processes = zombie_killer(processes);

	return processes;
}

int main()
{
	char line[MAX_LINE];
	Process *processes = NULL;

	while (1)
	{
		printf("hw1shell$ ");
		fgets(line, MAX_LINE, stdin);
		processes = execute_command(line, processes);
	}

	return 0;
}
