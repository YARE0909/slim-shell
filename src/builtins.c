#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

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

        pid_t pid = fork();

        if (pid == 0)
        {
            // child: run updater
            execl("/bin/sh", "sh", updater, NULL);
            perror("update");
            exit(1);
        }

        // parent: wait for update script
        int status;
        waitpid(pid, &status, 0);

        printf("[*] Update complete. Reloading Slim Shell...\n");

        // relaunch slim binary — THIS refreshes features immediately
        execl("/usr/local/bin/slim", "slim", NULL);

        perror("restart failed");
        return 1;
    }

    return 0;
}
