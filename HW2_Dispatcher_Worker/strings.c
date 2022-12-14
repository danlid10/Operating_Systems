#include "hw2.h"

// Split line to arguments delimited by whitespaces or semicolon.
void split_line(char line[], char *args[])
{
    char *token;
    int arg_count = 0;

    token = strtok(line, " ;\t\n\r");
    while (token != NULL)
    {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " ;\t\n\r");
    }

    args[arg_count] = NULL;
}
