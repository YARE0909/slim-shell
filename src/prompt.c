#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include "prompt.h"
#include "util.h"

char *build_prompt(void)
{
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));

    // ---- HARD LIMIT (300 chars max) ----
    char short_cwd[301];
    strncpy(short_cwd, cwd, 300);
    short_cwd[300] = '\0';

    char *username = getenv("USER");

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = (w.ws_col > 0) ? w.ws_col : 80;

    const char *BLUE = "\033[38;2;120;170;255m";
    const char *PURPLE = "\033[38;2;190;120;255m";
    const char *CYAN = "\033[38;2;120;255;230m";
    const char *GREEN = "\033[38;2;120;255;150m";
    const char *RESET = "\033[0m";

    static char prompt[2048];

    // ---- colored cwd will be max ~320 bytes ----
    char colored_cwd[350];
    snprintf(colored_cwd, sizeof(colored_cwd),
             "%s%s%s", PURPLE, short_cwd, RESET);

    // ---- NOW safe: left[] max size ≈ 350 + a few small parts ----
    char left[512];
    snprintf(left, sizeof(left),
             "%sYou at %s rn",
             BLUE, colored_cwd);

    // ---- TIME ----
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char tbuf[32];
    strftime(tbuf, sizeof(tbuf), "%H:%M:%S", t);

    char right[128];
    snprintf(right, sizeof(right),
             "%s ts %s rn %s",
             CYAN, tbuf, RESET);

    int left_v = visible_len(left);
    int right_v = visible_len(right);
    int spaces = width - left_v - right_v;
    if (spaces < 1)
        spaces = 1;

    // ---- FINAL PROMPT ----
    snprintf(prompt, sizeof(prompt),
             "%s%*s%s\n%s%s>%s ",
             left, spaces, "", right,
             GREEN, username ? username : "user", RESET);

    return prompt;
}

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
