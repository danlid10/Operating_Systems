#ifndef HW2_H
#define HW2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <limits.h>

#define MAX_THREADS 4096
#define MAX_COUNTERS 100
#define MAX_FILENAME 100
#define MAX_BASIC_CMD 10
#define MAX_BASIC_ARGS 200
#define MAX_CMD_LINE 1024
#define MAX_COUNTER_CHARS 100
#define TRUE 1
#define FALSE 0

// enums
enum worker_status {available, taken, done};
enum counter_action {inc, dec};

// structs
typedef struct Basic_CMD
{
    char command[MAX_BASIC_CMD];
    int num;
    struct Basic_CMD *next;
} Basic_CMD;

typedef struct Worker
{
    struct Basic_CMD *commands;
    enum worker_status state;
    int kill;
    struct timeval start_turnaround_time, end_turnaround_time;
    struct Worker *next;
} Worker;

// files
void init_counter_files(int num_counters);
void inc_dec_counter(int counter_n, enum counter_action inc_dec);

// strings
void split_line(char line[], char *args[]);

// dispatcher
void dispatcher(char cmdfilename[]);

// threads
pthread_t *init_threads(pthread_t *threads, int num_threads);
void wait_threads(pthread_t *threads, int num_threads);
void *routine(void *arg);
void execeute_worker(Worker *my_work);
Worker *find_available_job();
void Wait_for_pending_workers();
void insert_thread_killers(int num_threads);

// worker - basic commands linked list
Basic_CMD *create_worker_node(char *command, int num);
Basic_CMD *append_cmd_to_worker(Basic_CMD *worker, Basic_CMD *new_cmd);
void delete_worker_list(Basic_CMD *Worker);
Basic_CMD *array_to_cmd_list(char *args[]);

// work queue - workers linked list
Worker *create_queue_node(Basic_CMD *commands, int kill);
Worker *append_worker_to_queue(Worker *queue, Worker *new_worker);
void delete_queue_list(Worker *queue);

// stats
void create_thread_logfile(int thread_num);
void print_worker(Worker *worker, FILE *stream);
void append_thread_log(int thread_num, Worker *my_work, char start_end[]);
long long calc_runtime_ms(struct timeval *start_time, struct timeval *end_time);
void create_dispatcher_logfile();
void append_dispatcher_log(char line[]);
void delete_queue_and_write_stats();


// gloabal variables
extern Worker *work_queue;
extern pthread_mutex_t file_mutex;
extern pthread_mutex_t queue_mutex;
extern pthread_cond_t cond;
extern struct timeval start_time;
extern int log_enabled;

#endif