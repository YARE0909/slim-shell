#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Safe malloc wrapper for argument tokens
static char *dup_token(const char *start, int len)
{
    char *out = malloc(len + 1);
    if (!out)
        return NULL;

    memcpy(out, start, len);
    out[len] = '\0';
    return out;
}

int parse_pipeline(char *line, Pipeline *p)
{
    memset(p, 0, sizeof(Pipeline)); // Clear everything

    int cmd_index = 0;
    int argc = 0;

    while (*line)
    {
        // Skip whitespace
        while (*line == ' ' || *line == '\t')
            line++;

        if (*line == '\0')
            break;

        // ---------------------
        // PIPE |
        // ---------------------
        if (*line == '|')
        {
            p->cmds[cmd_index].argv[argc] = NULL;

            cmd_index++;
            argc = 0;

            if (cmd_index >= MAX_CMDS)
                break;

            line++;
            continue;
        }

        // ---------------------
        // OUTPUT REDIRECTION > or >>
        // ---------------------
        if (*line == '>')
        {
            int append = 0;
            line++;

            if (*line == '>') // >>
            {
                append = 1;
                line++;
            }

            // skip whitespace
            while (*line == ' ' || *line == '\t')
                line++;

            char *start = line;

            while (*line &&
                   *line != ' ' && *line != '\t' &&
                   *line != '|' && *line != '<' && *line != '>')
            {
                line++;
            }

            char *filename = dup_token(start, line - start);

            if (append)
                p->cmds[cmd_index].appendfile = filename;
            else
                p->cmds[cmd_index].outfile = filename;

            continue;
        }

        // ---------------------
        // INPUT REDIRECTION <
        // ---------------------
        if (*line == '<')
        {
            line++;

            while (*line == ' ' || *line == '\t')
                line++;

            char *start = line;

            while (*line &&
                   *line != ' ' && *line != '\t' &&
                   *line != '|' && *line != '<' && *line != '>')
            {
                line++;
            }

            p->cmds[cmd_index].infile = dup_token(start, line - start);
            continue;
        }

        // ---------------------
        // NORMAL TOKEN
        // ---------------------
        char *start = line;

        while (*line &&
               *line != ' ' && *line != '\t' &&
               *line != '|' && *line != '<' && *line != '>')
        {
            line++;
        }

        int len = line - start;
        char *tok = dup_token(start, len);

        if (argc < MAX_ARGS - 1)
        {
            p->cmds[cmd_index].argv[argc++] = tok;
        }
        else
        {
            free(tok); // Prevent leak if we exceed MAX_ARGS
        }
    }

    // Properly terminate last command argv
    p->cmds[cmd_index].argv[argc] = NULL;

    p->cmd_count = cmd_index + 1;
    return p->cmd_count;
}
