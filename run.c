#include "defs.h"

/* run command with no input mod */
int run(char **args) {
    pid_t cpid = fork();
    if (cpid == -1) {
        return -1;
    }
}

/* args > file */
void run_out(char **args, char *file) {
}

/* args >> file */
void run_app(char **args, char *file) {
}

/* args < file */
void run_inp(char **args, char *file) {
}
