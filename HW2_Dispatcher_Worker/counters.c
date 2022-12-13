#include "hw2.h"

void init_counter_files(int num_counters)
{
    FILE *fp;
    int i;
    char file_name[MAX_FILENAME];

    for (i = 0; i < num_counters; i++)
    {
        sprintf(file_name, "count%02d.txt", i);
        fp = fopen(file_name, "w");
        if (fp == NULL)
        {
            printf("[ERROR] File \"%s\": %s\n", file_name, strerror(errno));
            exit(-1);
        }
        fprintf(fp, "0");
        fclose(fp);
    }
}

void inc_dec_counter(int counter_n, enum counter_action inc_dec)
{
    FILE *fp;
    char file_name[MAX_FILENAME];
    long long num;
    char num_str[MAX_COUNTER_CHARS];

    sprintf(file_name, "count%02d.txt", counter_n);

    pthread_mutex_lock(&file_mutex);

    // read from counter n
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("[ERROR] File \"%s\": %s\n", file_name, strerror(errno));
        exit(-1);
    }
    fgets(num_str, MAX_FILENAME, fp);
    fclose(fp);

    // increase or decrease counter n
    num = atoi(num_str);
    switch (inc_dec)
    {
    case inc: num++; break;
    case dec: num--; break;
    }

    // write to counter n
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("[ERROR] File \"%s\": %s\n", file_name, strerror(errno));
        exit(-1);
    }
    fprintf(fp, "%lld", num);
    fclose(fp);

    pthread_mutex_unlock(&file_mutex);
}
