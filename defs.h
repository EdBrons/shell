#ifndef __DEFS_H
#define __DEFS_H

/* split line into the different program executions */
char **split_by_mods(char *line);

/* splits the substring into an array of strings for exec */
char **split_for_exec(char *substring);

int get_redir_mode(char *line);

void test();

/* run command with no input mod */
int run(char *args[]);

/* args > file */
int run_out(char *args[], char *file);

/* args >> file */
int run_app(char *args[], char *file);

/* args < file */
int run_inp(char *args[], char *file);

#endif
