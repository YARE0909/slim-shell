#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "input.h"

char *read_line(void)
{
    return readline(NULL);
}

int parse_line(char *line, char *argv[], int max_args)
{
    int argc = 0;
    char *token = strtok(line, " \t");

    while (token && argc < max_args - 1)
    {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }

    argv[argc] = NULL;
    return argc;
}

char *trim(char *str)
{
    while (*str == ' ' || *str == '\t') str++;
    if (*str == '\0') return str;

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
        *end-- = '\0';

    return str;
}
