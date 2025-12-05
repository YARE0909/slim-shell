#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "exec.h"

void run_external_command(char *argv[])
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        execvp(argv[0], argv);
        fprintf(stderr, "There ain't no '%s' command my guy\n", argv[0]);
        exit(1);
    }

    waitpid(pid, NULL, 0);
}
