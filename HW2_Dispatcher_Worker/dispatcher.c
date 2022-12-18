#include "hw2.h"

void dispatcher(char cmdfilename[], int log_enabled, struct timeval *start_time)
{
    char line[MAX_CMD_LINE];
    char *args[MAX_BASIC_ARGS];
    Basic_CMD *cmd_lst = NULL;
    Worker *new_worker = NULL;
    FILE *fp;

    fp = fopen(cmdfilename, "r");
    if (fp == NULL)
    {
        printf("[ERROR] File \"%s\": %s\n", cmdfilename, strerror(errno));
        exit(-1);
    }

    if (log_enabled == 1)
        create_dispatcher_logfile();

    while (fgets(line, MAX_CMD_LINE, fp) != NULL)
    {

        if (log_enabled == 1)
            append_dispatcher_log(line, start_time);

        split_line(line, args);

        if (args[0] == NULL) // skip empty line
            continue;

        if (!strcmp(args[0], "worker"))
        {
            cmd_lst = array_to_cmd_list(args);
            new_worker = create_worker_node(cmd_lst, FALSE);
            work_queue = append_worker_to_queue(new_worker);
            gettimeofday(&new_worker->start_turnaround_time, NULL);
            pthread_cond_broadcast(&cond); // wake up sleeping threads
        }
        else if (!strcmp(args[0], "dispatcher_msleep"))
        {
            usleep(1000 * atoi(args[1]));
        }
        else if (!strcmp(args[0], "dispatcher_wait"))
        {
            Wait_for_pending_workers();
        }
    }

    fclose(fp);
}