#include <stdio.h>

int visible_len(const char *s)
{
    int len = 0;

    while (*s)
    {
        if (*s == '\033')    // Beginning of ANSI escape sequence
        {
            s++;            // skip '\033'
            if (*s == '[')  // ESC[
            {
                s++;
                // Skip until 'm'
                while (*s && *s != 'm')
                    s++;
                if (*s == 'm')
                    s++;    // skip the 'm'
            }
        }
        else
        {
            len++;          // normal visible character
            s++;
        }
    }

    return len;
}
