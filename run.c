#include "defs.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int run_progs(struct prog_info *progs, int progc) {
    int *pipe_filedes = malloc(sizeof(int) * 2 * progc);
    int *io_filedes = malloc(sizeof(int) * progc);

    /* open files we need for io redirection */
    for (int i = 0; i < progc; i++) {
        switch (progs[i].mode) {
            case REDIR_OUT:
                if ((io_filedes[i] = open(progs->file, O_WRONLY | O_CREAT | O_TRUNC)) < 0) {
                    perror("open");
                    return -1;
                }
                break;
            case REDIR_APP:
                if ((io_filedes[i] = open(progs->file, O_WRONLY | O_CREAT)) < 0) {
                    perror("open");
                    return -1;
                }
                break;
            case REDIR_INP:
                if ((io_filedes[i] = open(progs->file, O_RDONLY)) < 0) {
                    perror("open");
                    return -1;
                }
                break;
        }
    }

    /* create pipes we need for io redirection */
    for (int i = 0; i < progc; i++) {
        if ((pipe(pipe_filedes + (i * 2))) == -1) {
            perror("fork");
            return -1;
        }
    }

    /* run processes */
    int prev_pipe = 0;
    for (int i = 0; i < progc; i++) {
        switch (fork()) {
            case -1:
                perror("fork");
                return -1;
            case 0:
                if (prev_pipe) {
                    printf("%s is reading from pipe %d\n", progs[i].args[0], (i-1) * 2);
                    while ((dup2(pipe_filedes[(i-1) * 2], STDIN_FILENO) == -1) && (errno == EINTR))
                        ;
                }
                else if (progs[i].mode == REDIR_INP) {
                    while ((dup2(io_filedes[i], STDIN_FILENO) == -1) && (errno == EINTR))
                        ;
                }
                if (progs[i].pipe) {
                    printf("%s is writing to the pipe %d\n", progs[i].args[0], i * 2 + 1);
                    while ((dup2(pipe_filedes[i * 2 + 1], STDOUT_FILENO) == -1) && (errno == EINTR))
                        ;
                }
                else if ((progs[i].mode == REDIR_APP) | (progs[i].mode == REDIR_OUT)) {
                    while ((dup2(io_filedes[i], STDOUT_FILENO) == -1) && (errno == EINTR))
                        ;
                }
                execvp(progs[i].args[0], progs[i].args + 1);
                perror("exec");
                return -1;
            default:
                break;
        }
        prev_pipe = progs[i].pipe;
    }

    free(pipe_filedes);
    free(io_filedes);

    return 1;
}

void test() {
    struct prog_info p1 = { { "ls", "-l", NULL }, 2, REDIR_NONE, 1, "" };
    struct prog_info p2 = { { "grep", "x", NULL }, 2, REDIR_OUT, 0, "out" };
    struct prog_info ps[] = { p1, p2 };
    int l = sizeof(ps) / sizeof(ps[0]);
    run_progs(ps, l);
}
