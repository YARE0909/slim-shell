#ifndef INPUT_H
#define INPUT_H

char *read_line(void);
int parse_line(char *line, char *argv[], int max_args);
char *trim(char *str);

#endif
