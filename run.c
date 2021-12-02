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

    /* open file for io redirection */
    switch (p->mode) {
        case REDIR_OUT:
            if ((io_filedes = open(p->file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
                perror("open");
                return -1;
            }
            break;
        case REDIR_APP:
            if ((io_filedes = open(p->file, O_WRONLY | O_APPEND | O_CREAT, 0666)) < 0) {
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

    /* create pipes */
    if (pipe(current_pipe) == -1) {
        perror("fork");
        return -1;
    }

    switch (fork()) {
        case -1: /* error */
            perror("fork");
            return -1;
        case 0: /* child */
            /* change stdin of child */
            close(last_pipe[1]);
            if (p->mode == REDIR_INP) {
                while ((dup2(io_filedes, STDIN_FILENO) == -1) && (errno == EINTR))
                    ;
            } else if (p->read_pipe) {
                while ((dup2(last_pipe[0], STDIN_FILENO) == -1) && (errno == EINTR))
                    ;
            }
            else {
                close(last_pipe[0]);
            }

            /* change stdout of child */
            close(current_pipe[0]);
            if ((p->mode == REDIR_APP) | (p->mode == REDIR_OUT)) {
                while ((dup2(io_filedes, STDOUT_FILENO) == -1) && (errno == EINTR))
                    ;
            } else if (p->write_pipe) {
                while ((dup2(current_pipe[1], STDOUT_FILENO) == -1) && (errno == EINTR))
                    ;
            }
            else {
                close(current_pipe[1]);
            }

            /* execute comand */
            execvp(p->args[0], p->args);
            /* command not found */
            printf("mysh: command not found: %s\n", p->args[0]);

            return -1;
        default: /* parent */
            close(io_filedes);
            close(last_pipe[0]);
            close(last_pipe[1]);
            memcpy(last_pipe, current_pipe, sizeof(int) * 2);
            break;
    }

    return 1;
}
