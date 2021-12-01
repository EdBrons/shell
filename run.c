#include "defs.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int exec_prog(struct prog_info *p) {
    static int last_pipe[2] = { -1, -1 };
    int current_pipe[2];
    int io_filedes = -1;
    int child_status;

    if (p == NULL) {
        last_pipe[0] = -1;
        last_pipe[1] = -1;
        return 1;
    }

    switch (p->mode) {
        case REDIR_OUT:
            if ((io_filedes = open(p->file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
                perror("open");
                return -1;
            }
            break;
        case REDIR_APP:
            if ((io_filedes = open(p->file, O_WRONLY | O_CREAT, 0666)) < 0) {
                perror("open");
                return -1;
            }
            break;
        case REDIR_INP:
            if ((io_filedes = open(p->file, O_RDONLY)) < 0) {
                perror("open");
                return -1;
            }
            break;
    }

    if (pipe(current_pipe) == -1) {
        perror("fork");
        return -1;
    }

    /*
    for (int i = 0; i < p->argc; i++) {
        printf("arg %d: %s \n", i + 1, p->args[i]);
    }
    */

    switch (fork()) {
        case -1:
            perror("fork");
            return -1;
        case 0:
            if (p->mode == REDIR_INP) {
                while ((dup2(io_filedes, STDIN_FILENO) == -1) && (errno == EINTR))
                    ;
            } else {
                while ((dup2(last_pipe[0], STDIN_FILENO) == -1) && (errno == EINTR))
                    ;
            }

            if ((p->mode == REDIR_APP) | (p->mode == REDIR_OUT)) {
                while ((dup2(io_filedes, STDOUT_FILENO) == -1) && (errno == EINTR))
                    ;
            } else {
                while ((dup2(current_pipe[1], STDOUT_FILENO) == -1) && (errno == EINTR))
                    ;
            }
            execvp(p->args[0], p->args);
            perror("exec");
            return -1;
        default:
            break;
    }

    wait(&child_status);

    if (io_filedes > 0) {
        close(io_filedes);
    }
    if (last_pipe[0] == -1) {
        close(last_pipe[0]);
    }
    if (last_pipe[1] == -1) {
        close(last_pipe[1]);
    }

    memcpy(last_pipe, current_pipe, sizeof(int) * 2);
    return 1;
}
