#include "defs.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#define BSIZE 1024

int _run(char *args[], int in, int out) {
    int filedes[2];
    if (pipe(filedes) == -1) {
        perror("pipe");
        exit(1);
    }

    switch (fork()) {
        case -1: /* Error */
            perror("fork");
            break;
        case 0: /* Child - read from pipe */
            // while loops for accounting for interupts
            while ((dup2(in, STDIN_FILENO) == -1) && (errno == EINTR)) {}
            while ((dup2(out, STDOUT_FILENO) == -1) && (errno == EINTR)) {}
            execvp(args[0], args+1);
            perror("execvp");
            exit(EXIT_FAILURE);
            break;
        default: /* Parent - write to pipe */
            wait(0);
            break;
    }

    return 1;
}

/* run command with no input mod */
int run(char *args[]) {
    return _run(args, STDIN_FILENO, STDOUT_FILENO);
}

/* args > file */
int run_out(char *args[], char *file) {
    int out;
    if ((out = open(file, O_WRONLY | O_TRUNC | O_CREAT)) < 0) {
        perror("open");
        return -1;
    }
    return _run(args, STDIN_FILENO, out);
}

/* args >> file */
int run_app(char *args[], char *file) {
    int out;
    if ((out = open(file, O_WRONLY | O_APPEND | O_CREAT)) < 0) {
        perror("open");
        return -1;
    }
    return _run(args, STDIN_FILENO, out);
}

/* args < file */
int run_inp(char *args[], char *file) {
    int in;
    if ((in = open(file, O_RDONLY)) < 0) {
        perror("open");
        return -1;
    }
    return _run(args, in, STDOUT_FILENO);
}

void test() {
    char *args[] = { "ls", "-l", "-a", NULL };
    run_out(args, "outfile");
    run_app(args, "outfile");
}
