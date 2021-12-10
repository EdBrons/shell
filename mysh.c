#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <fcntl.h>
#include "defs.h"

char *prompt = "mysh$";

void print_prompt() {
    printf("%s", prompt);
}

int main(int argc, char *argv[]) {
    char usr_input[LINELEN];
    int args_count = 0;
    struct prog_info p;

    while (1) {
        print_prompt();

        /* get input */
        
        if (fgets(usr_input, LINELEN, stdin) == NULL) {
            break;
        }
        else if (strncmp(usr_input, "\n", 1) == 0) {
            continue;
        }

        get_next_prog(&p, usr_input);
        if(strncmp(p.args[0], "exit", 4) == 0 ){
            break;
        }

        do {
            exec_prog(&p);
            args_count++;
        } while (get_next_prog(&p, NULL) > 0);
        /* wait for children */
        for (; args_count > 0; args_count--) {
            wait(NULL);
        }
        fflush(stdout);
    } 
    //printf("\nexit\n");
}

int get_next_prog(struct prog_info *p, char *line) {
    static char *save_ptr;
    p->read_pipe = 1;
    p->write_pipe = 1;
    if (line != NULL) {
        save_ptr = NULL;
        p->read_pipe = 0;
    }
    char *sub_str = strtok_r(line, "|", &save_ptr);
    if (sub_str == NULL) {
        return -1;
    } else if (sub_str[strlen(sub_str) - 1] == '\n') {
        sub_str[strlen(sub_str) - 1] = '\0';
        p->write_pipe = 0;
    }

    p->in_mode = REDIR_NONE;
    p->out_mode = REDIR_NONE;
    p->argc = 0;
    int file_next_in = 0;
    int file_next_out = 0;
    char *word = strtok(sub_str, " ");

    do {
        if (p->argc >= MAX_ARGC) {
            break;
        } else if (file_next_in) {
            strncpy(p->in_file, word, LINELEN);
            file_next_in = 0;

        } else if (file_next_out) {
            strncpy(p->out_file, word, LINELEN);
            file_next_out = 0;

        } else if (strncmp(word, ">", LINELEN) == 0) {
            p->out_mode = REDIR_OUT;
            file_next_out = 1;

        } else if (strncmp(word, ">>", LINELEN) == 0) {
            p->out_mode = REDIR_APP;
            file_next_out = 1;
        } else if (strncmp(word, "<", LINELEN) == 0) {
            p->in_mode = REDIR_INP;
            file_next_in = 1;
        } else {
            p->args[p->argc++] = word;
        }
    } while((word = strtok(NULL, " ")) != NULL);
    p->args[p->argc] = NULL;

    return 1;
}


int exec_prog(struct prog_info *p) {
    static int last_pipe[2] = { -1, -1 };
    int current_pipe[2];
    int out_file_des = -1;
    int in_file_des = -1; 

    /* open file for io redirection */

    if(p->in_mode == REDIR_INP){
                if ((in_file_des  = open(p->in_file, O_RDONLY)) < 0) {
                perror("open");
                return -1;
            }

    }
    switch (p->out_mode) {
        case REDIR_OUT:
            if ((out_file_des= open(p->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
                perror("open");
                return -1;
            }
            break;
        case REDIR_APP:
            if ((out_file_des = open(p->out_file, O_WRONLY | O_APPEND | O_CREAT, 0666)) < 0) {
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
            if (p->in_mode == REDIR_INP) {
                while ((dup2(in_file_des , STDIN_FILENO) == -1) && (errno == EINTR))
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
            if ((p->out_mode == REDIR_APP) | (p->out_mode == REDIR_OUT)) {
                while ((dup2(out_file_des , STDOUT_FILENO) == -1) && (errno == EINTR))
                    ;
            } else if (p->write_pipe) {
                while ((dup2(current_pipe[1], STDOUT_FILENO) == -1) && (errno == EINTR))
                    ;
            }
            else {
                close(current_pipe[1]);
            }

            /* execute comand */
            if(execvp(p->args[0], p->args) == -1){
                perror("execvp");
            }
            // 
            /* command not found */
            //printf("mysh: command not found: %s\n", p->args[0]);
            _exit(-1);
        default: /* parent */
            close(out_file_des );
            close(in_file_des );
            close(last_pipe[0]);
            close(last_pipe[1]);
            memcpy(last_pipe, current_pipe, sizeof(int) * 2);
            break;
    }

    return 1;
}


