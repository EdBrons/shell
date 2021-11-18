#include <stddef.h>
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char out_in[3] = ">>";
const char out_[2] = ">";
const char in_[2] = "<";
const char pipe_[2] = "|";
const char space_[2] = " ";


/* split line into the different program executions */
char **split_by_mods(char *line) {
        char *token; 
    char **vars = malloc(4096); 
    if(token = strtok(*substring, *out_in)){
        vars[0] = malloc(strlen(token));
        memcpy(vars[0], token, strlen(token));
        token = strtok(NULL, *out_in);
        vars[1] = malloc(strlen(token));
        memcpy(vars[1], token, strlen(token));

    }
    else if( token = strtok(*substring, *out_)){
        vars[0] = malloc(strlen(token));
        memcpy(vars[0], token, strlen(token));
        token = strtok(NULL, *out_);
        vars[1] = malloc(strlen(token));
        memcpy(vars[1], token, strlen(token));

    }
    else if(token = strtok(*substring, *in_)){
        vars[0] = malloc(strlen(token));
        memcpy(var[0], token, strlen(token));
        token = strtok(NULL, *in_);
        vars[1] = malloc(strlen(token));
        memcpy(vars[1], token, strlen(token));

    }
    else{

        token = strtok(*substring, *pipe_);
        int i = 0; 
        
        while(token!=NULL){
        vars[i] = malloc(strlen(token));
        memcpy(vars[i], token, strlen(token));
        token = strtok(NULL, *pipe_);
        }
	}


    return vars;
}

/* splits the substring into an array of strings for exec */
char **split_for_exec(char *substring) {
    char *token; 
    char **vars = malloc(strlen(substring) + 1); 
    if(token = strtok(*substring, *space_)){
        int i = 0; 
        while(token!=NULL){
        vars[i] = malloc(strlen(token));
        memcpy(vars[i], token, strlen(token));
        token = strtok(NULL, *space_);
    }
    }
    /* add a null byte at the end */
    //memcpy(vars[strlen(substring)], '\0', 1);

    return vars;
}
int get_redir_mode(char *line) {
    int redir_mode;
    redir_mode = 0; 

    
    return redir_mode;
}
