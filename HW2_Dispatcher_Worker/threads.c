#include "hw2.h"

void *routine(void *arg)
{
    Worker *my_work = NULL;
    int thread_num = *(int *)arg;

    free(arg);

    if (log_enabled == 1)
        create_thread_logfile(thread_num);

    do
    {
        pthread_mutex_lock(&queue_mutex);

        while ((my_work = find_available_job()) == NULL) // wait for available job
            pthread_cond_wait(&cond, &queue_mutex);

        my_work->state = taken;

        pthread_mutex_unlock(&queue_mutex);

        if (log_enabled == 1 && my_work->kill == FALSE)
            append_thread_log(thread_num, my_work, "START");

        execeute_worker(my_work);

        gettimeofday(&my_work->end_turnaround_time, NULL);

        my_work->state = done;

        if (log_enabled == 1 && my_work->kill == FALSE)
            append_thread_log(thread_num, my_work, "END");

    } while (my_work->kill == FALSE);

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

void execeute_worker(Worker *my_work)
{
    Basic_CMD *cur_cmd = my_work->commands;
    Basic_CMD *rep_start = NULL;
    int rep_count = 0;

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
        new_worker = create_queue_node(NULL, TRUE); // create thread killer node
        work_queue = append_worker_to_queue(work_queue, new_worker);
        pthread_cond_broadcast(&cond);
    }
}

void Wait_for_pending_workers()
{
    Worker *cur_work = work_queue;

    while (cur_work != NULL)
    {
        while (cur_work->state != done);
        cur_work = cur_work->next;
    }
}
