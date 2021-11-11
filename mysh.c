#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"

#define LINELEN 4096

#define REDIR_NONE 0
#define REDIR_OUT  1
#define REDIR_APP  2
#define REDIR_INP  3
#define REDIR_PIPE 4

char *prompt = "mysh$";

void print_prompt() {
    printf("%s", prompt);
}

int main(int argc, char *argv[]) {
    char in[LINELEN];
    int redir_mode = REDIR_NONE;

    while (1) {
        print_prompt();

        fgets(in, LINELEN, stdin);
        if (strncmp(in, "exit\n", 5) == 0) {
            break;
        }

        redir_mode = get_redir_mode(in);
        char **substrings = split_by_mods(in);

        if (redir_mode == REDIR_NONE) {
            char **args = split_for_exec(substrings[0]);
            run(args);
        }
        else if (redir_mode == REDIR_OUT) {
            char **args = split_for_exec(substrings[0]);
            char *file = split_for_exec(substrings[0]);
            run_out(args, file);
        }
        else if (redir_mode == REDIR_APP) {
            char **args = split_for_exec(substrings[0]);
            char *file = split_for_exec(substrings[0]);
            run_app(args, file);
        }
        else if (redir_mode == REDIR_INP) {
            char **args = split_for_exec(substrings[0]);
            char *file = split_for_exec(substrings[0]);
            run_inp(args, file);
        }
    } 
}
