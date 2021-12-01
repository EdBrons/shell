#ifndef __DEFS_H
#define __DEFS_H

#define REDIR_NONE 0
#define REDIR_OUT  1
#define REDIR_APP  2
#define REDIR_INP  3

#define LINELEN 4096
#define MAX_ARGC 10

struct prog_info {
    char *args[MAX_ARGC+1];
    int argc;
    int mode;
    int pipe;
    char file[LINELEN];
};

int get_next_prog(struct prog_info *p, char *line);
int exec_prog(struct prog_info *p);

#endif
