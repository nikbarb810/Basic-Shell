//
// Created by csd4690, Nikolas Barmparousis
//

#ifndef SHELL_MAIN_H
#define SHELL_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

//struct for a single command and its arguments
struct single_command {
    char* name;
    char** args;
    int num_args;
    struct redirect_info *redirects;
    struct single_command* next;
};


//struct for the command sequence
struct command_sequence {
    struct single_command *head;
    int num_commands;
    int piped;
};

//struct for the redirection info
struct redirect_info {
    char *file_in;
    char *file_out;
    int append;
};

//function to create new single command
struct single_command* create_single_command(char* command);

//function to create new piped command
struct command_sequence* create_piped_command(char *command);

//function to create new redirect info, and update the string accordingly
struct redirect_info *tokenize_redirection(char *command);

//function to insert new single command to the command sequence
void insert_single_command(struct single_command **root, struct single_command *new_command);

//function to free new single command
void free_single_command(struct single_command* command);

//function initializes command sequence(command seqs array)
void init_comm_seqs_arr(struct command_sequence *arr, int sz);

//function to free command sequence(command seqs array)
void free_command_sequence_arr(struct command_sequence *arr, int sz);

//read commands from the user, both single and piped
//fills the command seqs array
int read_commands(struct command_sequence* command_sequence);

//function executes a single command, whether piped or not
void exec_command(struct command_sequence command_seq);

//function executes a single command, based on redirections
//restoring the original fds after the command is executed
void call_exec(struct single_command *command);

//display prompt
void type_prompt();

#endif //SHELL_MAIN_H
