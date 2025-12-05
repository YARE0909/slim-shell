#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // fork, execvp, chdir, getcwd
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // waitpid
#include <sys/ioctl.h>
#include <time.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void print_prompt(void);
int read_line(char *buffer, size_t size);
int parse_line(char *line, char *argv[], int max_args);
int handle_builtin(char *argv[]);
void run_external_command(char *argv[]);

char *trim(char *str);
int visible_len(const char *s);
void print_banner(void);

int main(void)
{
    print_banner();

    char line[MAX_LINE];
    char *argv[MAX_ARGS];

    while (1)
    {

        print_prompt();

        if (!read_line(line, sizeof(line)))
        {
            printf("\n");
            break;
        }

        char *clean = trim(line);
        if (clean[0] == '\0')
        {
            continue;
        }

        int argc = parse_line(line, argv, MAX_ARGS);
        if (argc == 0)
        {
            continue;
        }

        if (handle_builtin(argv))
        {
            continue;
        }

        run_external_command(argv);
    }

    return 0;
}

void print_prompt(void)
{
    char cwd[4096];
    char *username = getenv("USER");
    getcwd(cwd, sizeof(cwd));

    // Terminal width
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = (w.ws_col > 0) ? w.ws_col : 80;

    // ---------- BEAUTIFUL TEXT COLORS (NO BACKGROUNDS) ----------
    const char *BLUE = "\033[38;2;120;170;255m";   // soft neon blue
    const char *PURPLE = "\033[38;2;190;120;255m"; // lavender purple
    const char *CYAN = "\033[38;2;120;255;230m";   // teal/cyan
    const char *GREEN = "\033[38;2;120;255;150m";  // mint green
    const char *RESET = "\033[0m";

    // ---------- BUILD LEFT SIDE ----------
    char left[8192];
    snprintf(left, sizeof(left),
             "%sYou at %s%s%s rn",
             BLUE, PURPLE, cwd, RESET);

    // ---------- BUILD RIGHT SIDE ----------
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%H:%M:%S", t);

    char right[256];
    snprintf(right, sizeof(right),
             "%s ts %s rn %s",
             CYAN, timebuf, RESET);

    // ---------- CALCULATE SPACING (using visible lengths) ----------
    int left_v = visible_len(left);
    int right_v = visible_len(right);

    int spaces = width - left_v - right_v;
    if (spaces < 0)
        spaces = 1;

    // ---------- PRINT FIRST LINE ----------
    printf("%s%*s%s\n", left, spaces, "", right);

    // ---------- SECOND LINE ----------
    printf("%s%s>%s ", GREEN, username ? username : "user", RESET);
    fflush(stdout);
}

int read_line(char *buffer, size_t size)
{
    if (fgets(buffer, size, stdin) == NULL)
    {
        // NULL means EOF (Ctrl+D) or error
        return 0;
    }

    // Remove trailing newline, if any
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }

    return 1;
}

int parse_line(char *line, char *argv[], int max_args)
{
    int argc = 0;

    char *token = strtok(line, " \t");
    while (token != NULL && argc < max_args - 1)
    {
        // Skip empty tokens just in case
        if (strlen(token) > 0)
        {
            argv[argc++] = token;
        }
        token = strtok(NULL, " \t");
    }

    argv[argc] = NULL;
    return argc;
}

int handle_builtin(char *argv[])
{
    if (strcmp(argv[0], "exit") == 0)
    {
        printf("Until next time hb\n");
        exit(0);
    }

    if (strcmp(argv[0], "cd") == 0)
    {
        const char *dir;

        if (argv[1] == NULL)
        {
            // No argument -> go to HOME
            dir = getenv("HOME");
            if (dir == NULL)
            {
                fprintf(stderr, "cd: HOME not set\n");
                return 1;
            }
        }
        else
        {
            dir = argv[1];
        }

        if (chdir(dir) != 0)
        {
            perror("cd");
        }
        return 1;
    }

    if (strcmp(argv[0], "pwd") == 0)
    {
        char *cwd = getcwd(NULL, 0);
        if (cwd != NULL)
        {
            printf("%s\n", cwd);
            free(cwd);
        }
        else
        {
            perror("pwd");
        }
        return 1;
    }

    // Not a built-in command
    return 0;
}

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
        // Child process: replace with the command
        execvp(argv[0], argv);
        // If execvp returns, there was an error
        fprintf(stderr, "There ain't no '%s' command my guy\n", argv[0]);
        exit(1);
    }
    else
    {
        // Parent process: wait for child to finish
        int status;
        if (waitpid(pid, &status, 0) < 0)
        {
            perror("waitpid");
        }
    }
}

// HELPER FUNCTIONS
char *trim(char *str)
{
    // Remove leading spaces
    while (*str == ' ' || *str == '\t')
    {
        str++;
    }

    // If the string became empty:
    if (*str == '\0')
        return str;

    // Remove trailing spaces
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
    {
        *end-- = '\0';
    }

    return str;
}

int visible_len(const char *s)
{
    int len = 0;
    while (*s)
    {
        if (*s == '\033')
        { // Skip ESC [
            while (*s && *s != 'm')
                s++; // Skip until end of color code
            if (*s)
                s++; // Skip 'm'
        }
        else
        {
            len++;
            s++;
        }
    }
    return len;
}

void print_banner(void) {
    const char *BLUE   = "\033[38;2;120;170;255m";
    const char *PURPLE = "\033[38;2;190;120;255m";
    const char *CYAN   = "\033[38;2;120;255;230m";
    const char *RESET  = "\033[0m";

    printf("\n");

    printf("%s  ____                        __                              \n", BLUE);
    printf(" /  _/__ _  __ _  ___ _  ___ / (_)_ _    __ __  ___  ___  ___ _ \n");
    printf("%s _/ //  ' \\/  ' \\/ _ `/ (_-</ / /  ' \\  / // / / _ `/ _ \\/ _ `/\n", PURPLE);
    printf("%s/___/_/_/_/_/_/_/\\_,_/ /___/_/_/_/_/_/  \\_,_/  \\_, /_//_/\\_, / \n", CYAN);
    printf("                                              /___/     /___/  %s\n\n", RESET);
}
