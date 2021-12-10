#ifndef __DEFS_H
#define __DEFS_H

#define REDIR_NONE 0
#define REDIR_OUT  1
#define REDIR_APP  2
#define REDIR_INP  3

#define LINELEN 4096
#define MAX_ARGC 10

#define 

struct prog_info {
    char *args[MAX_ARGC+1];
    int argc;
    /* indicates whether the program reads from a pipe */
    int read_pipe;
    /* indicates whether the program writes to a pipe */
    int write_pipe;
    /* indicates whether the process has a >, >>, <, or no io redirection */
    int in_mode;
    int out_mode;
    /* the file the process possible writes to or reads from */
    char file[LINELEN];
};

/* parses a single program execution from line */
int get_next_prog(struct prog_info *p, char *line);

/* executes the program p */
int exec_prog(struct prog_info *p);

#endif
