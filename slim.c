#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <time.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS 64

char *build_prompt(void);
char *read_line(void); // UPDATED
int parse_line(char *line, char *argv[], int max_args);
int handle_builtin(char *argv[]);
void run_external_command(char *argv[]);
char *trim(char *str);
int visible_len(const char *s);
void print_banner(void);

int main(void)
{
    print_banner();

    using_history(); // enable history

    char *line;
    char *argv[MAX_ARGS];

    while (1)
    {
        char *prompt = build_prompt();
        char *line = readline(prompt);

        if (!line)
        {
            printf("\n");
            break;
        }

        char *clean = trim(line);
        if (clean[0] == '\0')
        {
            free(line);
            continue;
        }

        add_history(clean); // Add command to history

        int argc = parse_line(clean, argv, MAX_ARGS);
        if (argc == 0)
        {
            free(line);
            continue;
        }

        if (!handle_builtin(argv))
        {
            run_external_command(argv);
        }

        free(line);
    }

    return 0;
}

// -----------------------------------------------------

char *build_prompt(void)
{
    char cwd[4096];
    char *username = getenv("USER");
    getcwd(cwd, sizeof(cwd));

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col > 0 ? w.ws_col : 80;

    const char *BLUE = "\033[38;2;120;170;255m";
    const char *PURPLE = "\033[38;2;190;120;255m";
    const char *CYAN = "\033[38;2;120;255;230m";
    const char *GREEN = "\033[38;2;120;255;150m";
    const char *RESET = "\033[0m";

    static char prompt[8192];

    // Build your two-line prompt exactly like before
    char left[4096];
    snprintf(left, sizeof(left),
             "%sYou at %s%s%s rn",
             BLUE, PURPLE, cwd, RESET);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timebuf[32];
    strftime(timebuf, sizeof(timebuf), "%H:%M:%S", t);

    char right[256];
    snprintf(right, sizeof(right),
             "%s ts %s rn %s",
             CYAN, timebuf, RESET);

    int left_v = visible_len(left);
    int right_v = visible_len(right);

    int spaces = width - left_v - right_v;
    if (spaces < 1)
        spaces = 1;

    // Compose full prompt
    snprintf(prompt, sizeof(prompt),
             "%s%*s%s\n%s%s>%s ",
             left, spaces, "", right,
             GREEN, username ? username : "user", RESET);

    return prompt;
}

// -----------------------------------------------------

// NEW: using readline()
char *read_line(void)
{
    char *input = readline(""); // prompt already printed above
    return input;               // readline() returns malloc'd memory
}

// -----------------------------------------------------

int parse_line(char *line, char *argv[], int max_args)
{
    int argc = 0;
    char *token = strtok(line, " \t");

    while (token && argc < max_args - 1)
    {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }

    argv[argc] = NULL;
    return argc;
}

// -----------------------------------------------------

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

// -----------------------------------------------------

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
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}

// -----------------------------------------------------

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

int visible_len(const char *s)
{
    int len = 0;

    while (*s)
    {
        if (*s == '\033')
        {
            while (*s && *s != 'm')
                s++;
            if (*s)
                s++;
        }
        else
        {
            len++;
            s++;
        }
    }
    return len;
}

// -----------------------------------------------------

void print_banner(void)
{
    const char *BLUE = "\033[38;2;120;170;255m";
    const char *PURPLE = "\033[38;2;190;120;255m";
    const char *CYAN = "\033[38;2;120;255;230m";
    const char *RESET = "\033[0m";

    printf("\n");

    printf("%s  ____                        __                              \n", BLUE);
    printf(" /  _/__ _  __ _  ___ _  ___ / (_)_ _    __ __  ___  ___  ___ _ \n");
    printf("%s _/ //  ' \\/  ' \\/ _ `/ (_-</ / /  ' \\  / // / / _ `/ _ \\/ _ `/\n", PURPLE);
    printf("%s/___/_/_/_/_/_/_/\\_,_/ /___/_/_/_/_/_/  \\_,_/  \\_, /_//_/\\_, / \n", CYAN);
    printf("                                              /___/     /___/  %s\n\n", RESET);
}
