#ifndef PIPELINE_H
#define PIPELINE_H

#define MAX_ARGS 64
#define MAX_CMDS 16

// One command in a pipeline
typedef struct {
    char *argv[MAX_ARGS];   // arguments
    char *infile;           // < redirection
    char *outfile;          // > redirection
    char *appendfile;       // >> redirection
} Cmd;

// Entire pipeline
typedef struct {
    Cmd cmds[MAX_CMDS];
    int cmd_count;
} Pipeline;

// Functions
int parse_pipeline(char *line, Pipeline *p);
void run_pipeline(Pipeline *p);
void free_pipeline(Pipeline *p);

#endif
