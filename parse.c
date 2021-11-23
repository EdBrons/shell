#include <stddef.h>
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *out_in = ">>";
const char *out_ = ">";
const char *in_ = "<";
const char *pipe_ = "|";
const char *space_ = " ";


/* split line into the different program executions */

void split_by_pipe(char *line){
    char *token; 
    token = strtok(line, "|");
    while(token!=NULL){
        //printf(" %s \n", token);
        split_by_mods(token);
        token = strtok_r(NULL, "|", token);
    }

} 

void split_by_mods(char *line){
    char *token;
    token = strtok(line, ">>");
    token = strtok(line, ">");
    token = strtok(line, "<");
    token = strtok(line, " ");
    printf(" %s \n", token);

}




void testParse(){
    char testString[4096] = "ls l a | grep x infile | exit";
    split_by_pipe(testString);
}

// char **split_by_mods(char *line) {
//         char *token; 
//     char **vars = malloc(4096); 
//     if(token = strtok(*substring, *out_in)){
//         vars[0] = malloc(strlen(token));
//         memcpy(vars[0], token, strlen(token));
//         vars[1] = malloc(strlen(out_in));
//         memcpy(vars[1], out_in, strlen(out_in));
//         token = strtok(NULL, *out_in);
//         vars[2] = malloc(strlen(token));
//         memcpy(vars[2], token, strlen(token));

//     }
//     else if( token = strtok(*substring, *out_)){
//         vars[0] = malloc(strlen(token));
//         memcpy(vars[0], token, strlen(token));
//         vars[1] = malloc(strlen(out_in));
//         memcpy(vars[1], out_in, strlen(out_in));
//         token = strtok(NULL, *out_);
//         vars[2] = malloc(strlen(token));
//         memcpy(vars[2], token, strlen(token));

//     }
//     else if(token = strtok(*substring, *in_)){
//         vars[0] = malloc(strlen(token));
//         memcpy(var[0], token, strlen(token));
//         vars[1] = malloc(strlen(out_in));
//         memcpy(vars[1], out_in, strlen(out_in));
//         token = strtok(NULL, *in_);
//         vars[2] = malloc(strlen(token));
//         memcpy(vars[2], token, strlen(token));

//     }
//     /*
//     else{

//         token = strtok(*substring, *pipe_);
//         int i = 0; 
//         while(token!=NULL){
//         vars[i] = malloc(strlen(token));
//         memcpy(vars[i], token, strlen(token));
//         token = strtok(NULL, *pipe_);
//         }
// 	}
//     */
//     else{
//         vars[0] = malloc(strlen(token));
//         memcpy(vars[0], line, strlen(line));

//     }


//     return vars;
// }

// /* splits the substring into an array of strings for exec */
// char **split_for_exec(char *substring) {
//     char **token; 
//     char **vars = malloc(strlen(substring)); 
//     if(token = strtok(*substring, *space_)){
//         int i = 0; 
//         while(token!=NULL){
//         vars[i] = malloc(strlen(token));
//         memcpy(vars[i], token, strlen(token));
//         token = strtok(NULL, *space_);

//     }
//     }
//     /* add a null byte at the end */
//     //memcpy(vars[strlen(substring)], '\0', 1);
//     // returns an array of individual commands

//     return vars;
// }

// // takes a line input and returns the mode
// char *get_redir_mode(char *line) {
//     if(line == ">>" ){
//         return "REDIR_APP";
//         }
//     if(line == ">" ){
//         return "REDIR_OUT";
//                 }
//     if(line == "<" ){
//         return  "REDIR_INP";
//         }

//     return "REDIR_NONE";
// }

// struct prog_info *get_progs(char *line){
//     struct prog_info prog_array[4096];
//     int prog_counter = 0;
//     char **token = split_by_pipe(line); // split by pipe 
//     char **vars; 
//     int len = int(sizeof(token) / sizeof(token[0])); 
//     for(int i = 0; i < len; i++){
//         char **by_mode = split_by_mods(token[i]); // split by mods
//         int sub_len = int(sizeof(by_mode)/sizeof(by_mode[0]));
//         //for(int k = 0; k <sub_len; k++){

//        // }
//         if(sub_len == 3){
//             char **exec_args = split_for_exec(by_mode[0]); // split by space
//                 struct prog_info prog; 
//                 prog.args = *exec_args;
//                 prog.argc = 0;
//                 prog.mode = get_redir_mode(by_mode[1]);
//                 prog.pipe = (len>1 && i < len - 1) ? 1: 0;
//                 prog.file = by_mode[2];

//             prog_array[prog_counter] = prog; 
//             /*
//             prog_info_array[prog_counter] = malloc(strlen(prog));
//             memcpy(prog_info_array[prog_counter]), prog, st
//             */
//         }
//         else{
//                 char **exec_args = split_for_exec(by_mode[0]); // split by space
//                 struct prog_info prog; 
//                 prog.args = *exec_args;
//                 prog.argc = 0;
//                 prog.mode = "REDIR_NONE";
//                 prog.pipe = (len>1 && i < len - 1) ? 1: 0;
//                 prog.file = "";

//             prog_array[prog_counter] = prog; 
//         }

//         //return prog_info;
//     }
//     // pass into pipe, return an array of substrings by separated by pipe
//     // pass the string into get mode, return array of strings separated by mode
//     // pass to for spec, return strings separated by space

//     return prog_array;
// }

// void test(){
//     char *trial = "ls l | grep x < infile ";
//     get_progs(trial);
// }

/*
void test() {
    struct prog_info p1 = { { "ls", "-l", NULL }, 2, REDIR_NONE, 1, "" }; // ls l | ""
    struct prog_info p2 = { { "grep", "x", NULL }, 2, REDIR_INP, 0, "infile" }; grep x < infile
    struct prog_info ps[] = { p1, p2 };
    int l = sizeof(ps) / sizeof(ps[0]);
    run_progs(ps, l);
}
*/

/*
struct prog_info {
    char *args[MAX_ARGC];
    int argc;
    int mode;
    int pipe;
    char *file;
};
*/
