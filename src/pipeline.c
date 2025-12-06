#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "pipeline.h"
#include "exec.h"

void run_pipeline(Pipeline *p)
{
    int num = p->cmd_count;
    int pipes[num - 1][2];

    // Create pipes
    for (int i = 0; i < num - 1; i++)
        pipe(pipes[i]);

    for (int i = 0; i < num; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            // --- INPUT ---
            if (p->cmds[i].infile)
            {
                int fd = open(p->cmds[i].infile, O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (i > 0)
                dup2(pipes[i - 1][0], STDIN_FILENO);

            // --- OUTPUT ---
            if (p->cmds[i].appendfile)
            {
                int fd = open(p->cmds[i].appendfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (p->cmds[i].outfile)
            {
                int fd = open(p->cmds[i].outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (i < num - 1)
                dup2(pipes[i][1], STDOUT_FILENO);

            // Close all pipe fds
            for (int j = 0; j < num - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(p->cmds[i].argv[0], p->cmds[i].argv);
            perror("exec failed");
            exit(1);
        }
    }

    // Parent closes pipes
    for (int i = 0; i < num - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num; i++)
        wait(NULL);
}

void free_pipeline(Pipeline *p)
{
    for (int i = 0; i < p->cmd_count; i++)
    {
        Cmd *c = &p->cmds[i];

        // free argv tokens
        for (int j = 0; c->argv[j] != NULL; j++)
            free(c->argv[j]);

        if (c->infile)
            free(c->infile);
        if (c->outfile)
            free(c->outfile);
        if (c->appendfile)
            free(c->appendfile);
    }
}