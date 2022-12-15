#include "hw2.h"

void *routine(void *arg)
{
    Worker *my_work = NULL;
    int thread_num = *(int *)arg;

    free(arg);

    // Create thread log file
    if (log_enabled == 1)
        create_thread_logfile(thread_num);

    do
    {
        // lock the queue mutex to deny other threads access to the global work queue
        pthread_mutex_lock(&queue_mutex);

        while ((my_work = find_available_job()) == NULL) // wait for available job
            pthread_cond_wait(&cond, &queue_mutex);      // wait for broadcast from the dispatcher

        my_work->state = taken; // mark the found available job as 'taken'

        // unlock the queue mutex to give other threads access to the global work queue
        pthread_mutex_unlock(&queue_mutex);

        execeute_worker(my_work, thread_num); // execute work

        my_work->state = done; // mark finished job as 'done'

    } while (my_work->kill == FALSE); // continue looping until get 'kill' job

    return NULL;
}

Worker *find_available_job()
{
    Worker *my_work = work_queue;

    while (my_work != NULL) // search for available job
    {
        if (my_work->state == available) // found available job
            return my_work;

        my_work = my_work->next;
    }
    return NULL;
}

void execeute_worker(Worker *my_work, int thread_num)
{
    Basic_CMD *cur_cmd = my_work->commands;
    Basic_CMD *rep_start = NULL;
    int rep_count = 0;

    // append START timing data to thread log file
    if (log_enabled == 1 && my_work->kill == FALSE)
        append_thread_log(thread_num, my_work, "START");

    while (cur_cmd != NULL)
    {
        if (!strcmp(cur_cmd->command, "msleep")) //  basic command - msleep
        {
            usleep(1000 * cur_cmd->num);
        }
        else if (!strcmp(cur_cmd->command, "increment")) //  basic command - increment
        {
            inc_dec_counter(cur_cmd->num, inc);
        }
        else if (!strcmp(cur_cmd->command, "decrement")) //  basic command - decrement
        {
            inc_dec_counter(cur_cmd->num, dec);
        }
        else if (!strcmp(cur_cmd->command, "repeat")) //  basic command - repeat
        {
            rep_start = cur_cmd->next;
            rep_count = cur_cmd->num - 1;
        }

        cur_cmd = cur_cmd->next;

        if (cur_cmd == NULL && rep_count > 0)
        {
            cur_cmd = rep_start;
            rep_count--;
        }
    }

    // append END timing data to thread log file
    if (log_enabled == 1 && my_work->kill == FALSE)
        append_thread_log(thread_num, my_work, "END");

    // update work's end turnaround time
    gettimeofday(&my_work->end_turnaround_time, NULL);
}

pthread_t *init_threads(pthread_t *threads, int num_threads)
{
    int i, error, *thread_num;
    threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    if (threads == NULL)
    {
        printf("[ERROR] malloc failed\n");
        exit(-1);
    }

    for (i = 0; i < num_threads; i++)
    {
        // allocate memory to pass the thread number to the thread routine
        thread_num = (int *)malloc(sizeof(int));
        if (thread_num == NULL)
        {
            printf("[ERROR] malloc failed\n");
            exit(-1);
        }
        *thread_num = i;

        error = pthread_create(&threads[i], NULL, &routine, thread_num);
        if (error != 0)
        {
            printf("[ERROR] pthread_create failed: error %d - %s\n", error, strerror(error));
            exit(-1);
        }
    }

    return threads;
}

void wait_threads(pthread_t *threads, int num_threads)
{
    int i, error;
    for (i = 0; i < num_threads; i++)
    {
        error = pthread_join(threads[i], NULL);
        if (error != 0)
        {
            printf("[ERROR] pthread_join failed: error %d - %s\n", error, strerror(error));
            exit(-1);
        }
    }
    free(threads);
}

void insert_thread_killers(int num_threads)
{
    int i;
    Worker *new_worker = NULL;

    for (i = 0; i < num_threads; i++)
    {
        new_worker = create_worker_node(NULL, TRUE); // create thread killer node
        work_queue = append_worker_to_queue(new_worker);
        pthread_cond_broadcast(&cond);
    }
}

void Wait_for_pending_workers()
{
    Worker *cur_work = work_queue;

    while (cur_work != NULL)
    {
        while (cur_work->state != done); // wait until work state is 'done'
        cur_work = cur_work->next;
    }
}
