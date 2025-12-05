#include <stdio.h>

int visible_len(const char *s)
{
    int len = 0;

    while (*s)
    {
        if (*s == '\033') {        // Skip ANSI escape sequences
            while (*s && *s != 'm')
                s++;
            if (*s) s++;
        } else {
            len++;
            s++;
        }
    }
    return len;
}
