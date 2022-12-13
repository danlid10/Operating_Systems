#include "hw2.h"

// Prints worker to FILE stream (stdout for printf)
void print_worker(Worker *worker, FILE *stream)
{
    Basic_CMD *cur_cmd = worker->commands;

    if (cur_cmd != NULL)
        fprintf(stream, "worker");
    while (cur_cmd != NULL)
    {
        fprintf(stream, " %s %d;", cur_cmd->command, cur_cmd->num);
        cur_cmd = cur_cmd->next;
    }
    fprintf(stream, "\n");
}

// Create log file for thread thread_num.
void create_thread_logfile(int thread_num)
{
    FILE *fp;
    char file_name[MAX_FILENAME];

    sprintf(file_name, "thread%04d.txt", thread_num);
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("[ERROR] File \"%s\": %s\n", file_name, strerror(errno));
        exit(-1);
    }
    fclose(fp);
}

// Append timing data to thread_num log file.
void append_thread_log(int thread_num, Worker *my_work, char start_end[])
{
    FILE *fp;
    char file_name[MAX_FILENAME];
    long long runtime_ms;
    struct timeval end_time;

    gettimeofday(&end_time, NULL);
    runtime_ms = calc_runtime_ms(&start_time, &end_time);

    sprintf(file_name, "thread%04d.txt", thread_num);
    fp = fopen(file_name, "a");
    if (fp == NULL)
    {
        printf("[ERROR] File \"%s\": %s\n", file_name, strerror(errno));
        exit(-1);
    }

    fprintf(fp, "TIME %lld: %s job ", runtime_ms, start_end);
    print_worker(my_work, fp);

    fclose(fp);
}

// Calculate runtime in ms from 2 timeval structs.
long long calc_runtime_ms(struct timeval *start_time, struct timeval *end_time)
{
    return (end_time->tv_sec - start_time->tv_sec) * 1000 + (end_time->tv_usec - start_time->tv_usec) / 1000;
}

// Create log file for the dispatcher.
void create_dispatcher_logfile()
{
    FILE *fp = fopen("dispatcher.txt", "w");
    if (fp == NULL)
    {
        printf("[ERROR] File \"%s\": %s\n", "dispatcher.txt", strerror(errno));
        exit(-1);
    }
    fclose(fp);
}

// Append timing data to the dispatcher log file.
void append_dispatcher_log(char line[])
{
    struct timeval end_time;
    long long runtime_ms;
    FILE *fp;

    gettimeofday(&end_time, NULL);
    runtime_ms = calc_runtime_ms(&start_time, &end_time);

    fp = fopen("dispatcher.txt", "a");
    if (fp == NULL)
    {
        printf("[ERROR] File \"%s\": %s\n", "dispatcher.txt", strerror(errno));
        exit(-1);
    }

    fprintf(fp, "TIME %lld: read cmd line: %s", runtime_ms, line);

    fclose(fp);
}

// Create and write timing data to stats file.
void write_workers_stats()
{
    Worker *cur_work = work_queue;
    long long total_runtime_ms, turnaround_runtime_ms;
    long long min_job = LLONG_MAX, max_job = 0, sum_jobs = 0, job_count = 0;
    float avg_job;
    struct timeval end_time;
    FILE *fp;

    while (cur_work != NULL)
    {
        turnaround_runtime_ms = calc_runtime_ms(&cur_work->start_turnaround_time, &cur_work->end_turnaround_time);

        job_count++;
        sum_jobs += turnaround_runtime_ms;
        if (turnaround_runtime_ms < min_job)
            min_job = turnaround_runtime_ms;
        if (turnaround_runtime_ms > max_job)
            max_job = turnaround_runtime_ms;

        cur_work = cur_work->next;
    }
    avg_job = (float)sum_jobs / job_count;

    fp = fopen("stats.txt", "w");
    if (fp == NULL)
    {
        printf("[ERROR] File \"%s\": %s\n", "stats.txt", strerror(errno));
        exit(-1);
    }

    gettimeofday(&end_time, NULL);
    total_runtime_ms = calc_runtime_ms(&start_time, &end_time);

    fprintf(fp, "total running time: %lld milliseconds\n", total_runtime_ms);
    fprintf(fp, "sum of jobs turnaround time: %lld milliseconds\n", sum_jobs);
    fprintf(fp, "min job turnaround time: %lld milliseconds\n", min_job);
    fprintf(fp, "average job turnaround time: %f milliseconds\n", avg_job);
    fprintf(fp, "max job turnaround time: %lld milliseconds\n", max_job);

    fclose(fp);
}