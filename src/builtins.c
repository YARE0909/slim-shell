#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "builtins.h"
#include "prompt.h"

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
            execl("/bin/sh", "sh", updater, NULL);
            perror("update");
            exit(1);
        }

        int status;
        waitpid(pid, &status, 0);

        printf("[*] Update complete. Reloading Slim Shell...\n");

        // Restart from the UPDATED binary in install directory
        char newbin[512];
        snprintf(newbin, sizeof(newbin),
                 "%s/.slim-shell/slim", home);

        execl(newbin, "slim", NULL);

        perror("restart failed");
        return 1;
    }

    if (strcmp(argv[0], "immaslimugng") == 0) {
        print_banner();
        return 1;
    }

    return 0;
}
