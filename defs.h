#ifndef __DEFS_H
#define __DEFS_H

#define REDIR_NONE 0
#define REDIR_OUT  1
#define REDIR_APP  2
#define REDIR_INP  3
#define REDIR_PIPE 4
#define REDIR_TOTAL 5

#define MAX_ARGC 10

struct prog_info {
    char *args[MAX_ARGC];
    int argc;
    int mode;
    int pipe;
    char *file;
};

/* split line into the different program executions */
char **split_by_mods(char *line);

/* splits the substring into an array of strings for exec */
char **split_for_exec(char *substring);

int get_redir_mode(char *line);

struct prog_info *get_progs(char *line);

int run_progs(struct prog_info *progs, int progc);

void test();

#endif
