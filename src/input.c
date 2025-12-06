#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "input.h"

char *read_line(void)
{
    return readline(NULL);
}

char *trim(char *str)
{
    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == '\0')
        return str;

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
        *end-- = '\0';

    return str;
}
