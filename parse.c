#include <stddef.h>
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_next_prog(struct prog_info *p, char *line) {
    static char *save_ptr;
    if (line != NULL) {
        save_ptr = NULL;
    }
    char *sub_str = strtok_r(line, "|\n\0", &save_ptr);
    if (sub_str == NULL) {
        return -1;
    }

    p->mode = REDIR_NONE;
    p->argc = 0;
    int file_next = 0;
    char *word = strtok(sub_str, " ");

    do {
        if (p->argc >= MAX_ARGC) {
            break;
        } else if (file_next) {
            strncpy(p->file, word, LINELEN);
            file_next = 0;
        } else if (strncmp(word, ">", LINELEN) == 0) {
            p->mode = REDIR_OUT;
            file_next = 1;
        } else if (strncmp(word, ">>", LINELEN) == 0) {
            p->mode = REDIR_APP;
            file_next = 1;
        } else if (strncmp(word, "<", LINELEN) == 0) {
            p->mode = REDIR_INP;
            file_next = 1;
        } else {
            p->args[p->argc++] = word;
        }
    } while((word = strtok(NULL, " ")) != NULL);
    p->args[p->argc] = NULL;

    return 1;
}
