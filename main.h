//
// Created by johndoe on 16/10/2023.
//

#ifndef SHELL_MAIN_H
#define SHELL_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//struct for a single command and its arguments
struct single_command {
    char* name;
    char** args;
    int num_args;
    struct single_command* next;
};


//struct for the command sequence
struct command_sequence {
    struct single_command *head;
    int num_commands;
    int piped;
};

//function to create new single command
struct single_command* create_single_command(char* command);

struct command_sequence* create_piped_command(char *command);

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

//display promt
void type_prompt();

#endif //SHELL_MAIN_H
