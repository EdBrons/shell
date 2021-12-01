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
        // printf("closing last_pipe { %d %d }\n", last_pipe[0], last_pipe[1]);
        close(last_pipe[0]);
        close(last_pipe[1]);
        last_pipe[0] = -1;
        last_pipe[1] = -1;
        return 1;
    }

    // printf("we are on: %s\n", p->args[0]);

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
    // printf("created current pipes: { %d, %d }\n", current_pipe[0], current_pipe[1]);

    switch (fork()) {
        case -1:
            perror("fork");
            return -1;
        case 0:
            if (p->mode == REDIR_INP) {
                // printf("reading from file %s\n", p->file);
                while ((dup2(io_filedes, STDIN_FILENO) == -1) && (errno == EINTR))
                    ;
            } else if (p->read_pipe) {
                // printf("reading from last pipe %d\n", last_pipe[0]);
                while ((dup2(last_pipe[0], STDIN_FILENO) == -1) && (errno == EINTR))
                    ;
            }

            if ((p->mode == REDIR_APP) | (p->mode == REDIR_OUT)) {
                // printf("writing to file %s\n", p->file);
                while ((dup2(io_filedes, STDOUT_FILENO) == -1) && (errno == EINTR))
                    ;
            } else if (p->write_pipe) {
                // printf("writing to current pipe %d\n", current_pipe[1]);
                while ((dup2(current_pipe[1], STDOUT_FILENO) == -1) && (errno == EINTR))
                    ;
            }

            execvp(p->args[0], p->args);
            perror("exec");
            return -1;
        default:
            if (p->write_pipe) {
                // printf("HERE: closing current pipe %d\n", current_pipe[1]);
                close(current_pipe[1]);
            }
            close(io_filedes);
            wait(&child_status);
            // printf("Closing last_pipe { %d %d }\n", last_pipe[0], last_pipe[1]);
            close(last_pipe[0]);
            // close(last_pipe[1]);
            // printf("Moving current pipe { %d } to old pipe.\n\n\n", current_pipe[0]);
            memcpy(last_pipe, current_pipe, sizeof(int) * 2);
            break;
    }

    return 1;
}
