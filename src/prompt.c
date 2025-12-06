#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

#include "util.h"
#include "colors.h"
#include "prompt.h"

char *build_prompt(void)
{
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));

    // limit cwd to avoid long prompts
    char short_cwd[301];
    strncpy(short_cwd, cwd, 300);
    short_cwd[300] = '\0';

    char *username = getenv("USER");

    static char prompt[2048];

    // ---- time ----
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char tbuf[32];
    strftime(tbuf, sizeof(tbuf), "%H:%M:%S", t);

    // ---- final prompt: simple, clean, no alignment ----
    snprintf(prompt, sizeof(prompt),
             "sup %s%s%s | you at %s%s%s rn | ts %s%s%s\n> ",
             GREEN, username ? username : "user", RESET,
             PURPLE, short_cwd, RESET,
             CYAN, tbuf, RESET);

    return prompt;
}

void print_banner(void)
{
    printf("\n");

    printf("%s  ____                        __                              \n", BLUE);
    printf(" /  _/__ _  __ _  ___ _  ___ / (_)_ _    __ __  ___  ___  ___ _ \n");
    printf("%s _/ //  ' \\/  ' \\/ _ `/ (_-</ / /  ' \\  / // / / _ `/ _ \\/ _ `/\n", PURPLE);
    printf("%s/___/_/_/_/_/_/_/\\_,_/ /___/_/_/_/_/_/  \\_,_/  \\_, /_//_/\\_, / \n", CYAN);
    printf("                                              /___/     /___/  %s\n\n", RESET);
}
