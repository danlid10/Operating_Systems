#include "hw2.h"

// insert basic command node to worker list at the end
Basic_CMD *append_cmd_to_worker(Basic_CMD *worker, Basic_CMD *new_cmd)
{
    Basic_CMD *last_node;

    if (worker == NULL) // list is empty
        worker = new_cmd;
    else
    {
        last_node = worker;
        while (last_node->next != NULL)
            last_node = last_node->next;
        last_node->next = new_cmd;
    }
    return worker;
}

Basic_CMD *create_worker_node(char *command, int num)
{
    Basic_CMD *new_cmd = (Basic_CMD *)malloc(sizeof(Basic_CMD));
    if (new_cmd == NULL)
    {
        printf("[ERROR] malloc failed\n");
        exit(1);
    }
    strncpy(new_cmd->command, command, MAX_BASIC_CMD);
    new_cmd->num = num;
    new_cmd->next = NULL;

    return new_cmd;
}

Basic_CMD *array_to_cmd_list(char *args[])
{
    int i;
    Basic_CMD *new_cmd, *cmd_lst = NULL;

    for (i = 1; args[i] != NULL; i += 2)
    {
        new_cmd = create_worker_node(args[i], atoi(args[i + 1]));
        cmd_lst = append_cmd_to_worker(cmd_lst, new_cmd);
    }

    return cmd_lst;
}

void delete_worker_list(Basic_CMD *worker)
{
    Basic_CMD *current = worker;
    Basic_CMD *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

Worker *create_queue_node(Basic_CMD *commands, int kill)
{
    Worker *new_worker = (Worker *)malloc(sizeof(Worker));
    if (new_worker == NULL)
    {
        printf("[ERROR] malloc failed\n");
        exit(1);
    }
    new_worker->commands = commands;
    new_worker->state = available; // always initialized to 'available'
    new_worker->kill = kill;
    new_worker->next = NULL;

    return new_worker;
}

// Insert worker node to queue list at the end
Worker *append_worker_to_queue(Worker *queue, Worker *new_worker)
{
    Worker *last_node;

    if (queue == NULL) // list is empty
        queue = new_worker;
    else
    {
        last_node = queue;
        while (last_node->next != NULL)
            last_node = last_node->next;
        last_node->next = new_worker;
    }
    return queue;
}

void delete_queue_list(Worker *queue)
{
    Worker *current = queue, *next;

    while (current != NULL)
    {
        next = current->next;
        delete_worker_list(current->commands);
        free(current);
        current = next;
    }
}
