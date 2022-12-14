#include "hw2.h"

// global variables
Worker *work_queue = NULL;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
struct timeval start_time;
int log_enabled;

int main(int argc, char *argv[])
{
    int num_counters, num_threads;
    pthread_t *threads = NULL;

    gettimeofday(&start_time, NULL);

    // check command line arguments
    if (argc != 5)
    {
        printf("[ERROR] Invalid command line arguments: Incorrect number of given parameters\n");
        return -1;
    }
    num_threads = atoi(argv[2]);
    if (num_threads > MAX_THREADS || num_threads < 1)
    {
        printf("[ERROR] Invalid command line arguments: num_threads (%d) should be in [1, %d]\n", num_threads, MAX_THREADS);
        return -1;
    }
    num_counters = atoi(argv[3]);
    if (num_counters > MAX_COUNTERS || num_counters < 1)
    {
        printf("[ERROR] Invalid command line arguments: num_counters (%d) should be in [1, %d]\n", num_counters, MAX_COUNTERS);
        return -1;
    }
    log_enabled = atoi(argv[4]);
    if (log_enabled != 0 && log_enabled != 1)
    {
        printf("[ERROR] Invalid command line arguments: log_enabled (%d) should be in [0,1]'\n", log_enabled);
        return -1;
    }

    // Initialization
    init_counter_files(num_counters);
    threads = init_threads(threads, num_threads);

    dispatcher(argv[1]);
    Wait_for_pending_workers();

    // Clean up
    insert_thread_killers(num_threads);
    wait_threads(threads, num_threads);
    delete_queue_and_write_stats();

    return 0;
}