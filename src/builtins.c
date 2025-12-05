#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"

int handle_builtin(char *argv[])
{
    if (strcmp(argv[0], "exit") == 0)
    {
        printf("Until next time hb\n");
        exit(0);
    }

    if (strcmp(argv[0], "cd") == 0)
    {
        const char *dir = argv[1] ? argv[1] : getenv("HOME");

        if (!dir)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }

        if (chdir(dir) != 0)
            perror("cd");

        return 1;
    }

    return 0;
}
