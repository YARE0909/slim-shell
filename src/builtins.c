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

    if (strcmp(argv[0], "update") == 0)
    {
        printf("[*] Updating Slim Shell...\n");

        const char *home = getenv("HOME");

        if (!home)
        {
            fprintf(stderr, "update: HOME not set\n");
            return 1;
        }

        char updater[512];
        snprintf(updater, sizeof(updater),
                 "%s/.slim-shell/scripts/update.sh", home);

        // Execute the script
        execl("/bin/sh", "sh", updater, NULL);

        perror("update");
        return 1;
    }

    return 0;
}
