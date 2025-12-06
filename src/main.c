#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "prompt.h"
#include "input.h"
#include "builtins.h"
#include "parser.h"
#include "pipeline.h"
#include "exec.h"

int main(void)
{
    print_banner();
    using_history();

    char *line;

    while (1)
    {
        char *prompt = build_prompt();
        line = readline(prompt);

        if (!line)
        {
            printf("\n");
            break;
        }

        char *clean = trim(line);

        if (*clean == '\0')
        {
            free(line);
            continue;
        }

        char *copy = strdup(clean); // <-- make a SAFE copy for parser
        if (!copy)
        {
            free(line);
            continue;
        }

        add_history(clean);

        // --------------------------------------------------
        // STEP 1: Copy line to detect builtins only
        // --------------------------------------------------
        char tmp[512];
        strncpy(tmp, clean, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';

        // Extract only first token
        char *cmd = strtok(tmp, " ");
        char *builtin_argv[2] = {cmd, NULL};

        if (handle_builtin(builtin_argv))
        {
            free(copy);
            free(line);
            continue;
        }

        // --------------------------------------------------
        // STEP 2: Parse pipeline (CANNOT MODIFY clean BEFORE THIS)
        // --------------------------------------------------
        Pipeline p;
        parse_pipeline(copy, &p);

        // --------------------------------------------------
        // STEP 3: Execute
        // --------------------------------------------------
        if (p.cmd_count == 1)
            run_external_command(p.cmds[0].argv);
        else
            run_pipeline(&p);

        // --------------------------------------------------
        // STEP 4: Free parsed tokens
        // --------------------------------------------------
        free_pipeline(&p);

        free(copy);
        free(line);
    }

    return 0;
}
