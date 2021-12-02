#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"

char *prompt = "mysh$";

void print_prompt() {
    printf("%s", prompt);
}

int main(int argc, char *argv[]) {
    char in[LINELEN];
    int c = 0;
    struct prog_info p;

    while (1) {
        print_prompt();

        if (fgets(in, LINELEN, stdin) == NULL) {
            break;
        }

        if (strncmp(in, "exit\n", 5) == 0) {
            break;
        }
        else if (strncmp(in, "\n", 1) == 0) {
            continue;
        }

        get_next_prog(&p, in);
        do {
            exec_prog(&p);
            c++;
        } while (get_next_prog(&p, NULL) > 0);
        for (; c > 0; c--) {
            wait(NULL);
        }
        // exec_prog(NULL);
        fflush(stdout);
    } 
}
