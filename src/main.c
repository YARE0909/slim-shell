#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "prompt.h"
#include "input.h"
#include "builtins.h"
#include "exec.h"

#define MAX_ARGS 64

int main(void)
{
    print_banner();

    using_history(); // Enable readline history

    char *line;
    char *argv[MAX_ARGS];

    while (1)
    {
        char *prompt = build_prompt();
        line = readline(prompt);

        if (!line) {
            printf("\n");
            break; // Ctrl+D
        }

        char *clean = trim(line);

        if (clean[0] == '\0') {
            free(line);
            continue;
        }

        add_history(clean);

        int argc = parse_line(clean, argv, MAX_ARGS);
        if (argc == 0) {
            free(line);
            continue;
        }

        if (!handle_builtin(argv))
            run_external_command(argv);

        free(line);
    }

    return 0;
}
