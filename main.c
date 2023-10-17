#include "main.h"




//function to create new single command
struct single_command* create_single_command(char *command) {

    char *dup_command = strdup(command);

    int num_args;


    //tokenize command based on space
    char *token = strtok(dup_command, " ");

    //copy using strcpy
    char *command_name =  strdup(token);

    //initalize str array to store args
    char **command_args = (char **) malloc(100 * sizeof(char *));


    int i = 0;
    while(token) {
        //copy using strcpy
        command_args[i] = strdup(token);

        i++;
        token = strtok(NULL, " ");
    }

    //add NULL
    command_args[i++] = NULL;

    num_args = i;

    struct single_command *new_command = (struct single_command *) malloc(sizeof(struct single_command));

    new_command->name = command_name;
    new_command->args = command_args;
    new_command->num_args = num_args;
    new_command->next = NULL;


    return new_command;
}

//function to create a new piped command
struct command_sequence* create_piped_command(char *command) {

    char *dup_command = strdup(command);

    int num_single_commands = 0;

    //tokenize command based on |
    char *token = strtok_r(dup_command, "|", &dup_command);

    struct command_sequence *new_command_sequence = (struct command_sequence *) malloc(sizeof(struct command_sequence));

    //create new single command
    struct single_command *head = create_single_command(token);
    new_command_sequence->head = head;

    while(token) {
        num_single_commands++;
        token = strtok_r(dup_command, "|", &dup_command);

        if(token == NULL) {break;}

        //create new single command
        struct single_command *new_command = create_single_command(token);
        //insert it to head list
        insert_single_command(&head, new_command);
    }

    new_command_sequence->num_commands = num_single_commands;

    return new_command_sequence;
}

//function inserts a new single_command to the end of the given root
void insert_single_command(struct single_command **root, struct single_command *new_command) {
    struct single_command *curr = *root;

    if(curr == NULL) {
        *root = new_command;
        return;
    }

    while(curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = new_command;
}

//function to free new single command
void free_single_command(struct single_command* command) {
    free(command->name);
    command->name = NULL;

    for(int i = 0; i < command->num_args; i++) {
        if(command->args[i] != NULL) {
            free(command->args[i]);
            command->args[i] = NULL;
        }
    }

    free(command->args);
    command->args = NULL;

    //TODO FREE NEXT IN CASE OF PIPED COMMANDS
//    while(command->next) {
//        free_single_command(command->next);
//    }

    free(command);
}



//function to create new command sequence array
//array will be statically allocated 100 command_sequences
void init_comm_seqs_arr(struct command_sequence *arr, int sz) {

    int i;
    for(i = 0; i < sz; i++) {
        arr[i].num_commands = 0;
        arr[i].piped = 0;
        arr[i].head = NULL;
    }


}



void free_command_sequence_arr(struct command_sequence *arr, int sz) {
    int i;
    //loop through each index of the array
    for(i = 0; i < sz; i++) {
        //if command exists
        if(arr[i].head != NULL) {
            free_single_command(arr[i].head);
            arr[i].head = NULL;
            arr[i].num_commands = 0;
            arr[i].piped = 0;
        }
    }
}


int read_commands(struct command_sequence *command_arr) {


    char *input = (char *) malloc(200 * sizeof(char));
    char *token;
    int i = 0;

    //read input from the user
    while (1) {
        int c = getchar();
        input[i++] = (char) c;
        if (c == '\n') {
            input[--i] = '\0';
            break;
        }
    }


    //duplicate input
    char *input_dup = strdup(input);

    //tokenize input based on ;
    token = strtok(input_dup, ";");
    i = 0;

    //initalize str array to store commands
    char **commands = (char **) malloc(100 * sizeof(char *));

    //loop through input
    while (token) {
        //cpy using strcpy
        commands[i] = (char *) malloc(100 * sizeof(char));
        strcpy(commands[i], token);

        i++;
        token = strtok(NULL, ";");
    }


    int num_sep_commands = i;

    //loop through current commands
    for(i = 0; i < num_sep_commands; i++) {


        if(strchr(commands[i], '|')) {
            //piped command
            command_arr[i] = *create_piped_command(commands[i]);
            command_arr[i].piped = 1;
        } else {
            //non-piped command
            command_arr[i].head = create_single_command(commands[i]);
            command_arr[i].piped = 0;
            command_arr[i].num_commands = 1;
        }
    }

    return num_sep_commands;
}


void exec_command(struct command_sequence command_seq) {

    if(command_seq.piped) {

    } else {
        //non-piped command

        //cd, change dir to first arg
        //quit, exit gracefully
        //fork, execvp, wait
        if(strcmp(command_seq.head->name, "cd") == 0) {
            chdir(command_seq.head->args[1]);
        } else if(strcmp(command_seq.head->name, "quit") == 0) {
            exit(0);
        } else {
            //error handling
            pid_t pid = fork();

            if(pid == 0) {
                //child
                execvp(command_seq.head->name, command_seq.head->args);
            } else {
                //parent
                wait(NULL);
            }
        }
    }

}

//display promt
void type_prompt() {
    printf("4690-hy-345sh@%s:%s$ ", getenv("USER"), getcwd(NULL, 1024));
}

int main() {

    int sz = 100;
    struct command_sequence commands_arr[sz];
    int i;
    int num_commands;

    init_comm_seqs_arr(commands_arr, sz);

    //loop
    while(1) {

        //display prompt
        type_prompt();

        //read command
        num_commands = read_commands(commands_arr);

        //execute commands
        for(i = 0; i < num_commands; i++) {
            exec_command(commands_arr[i]);
        }

        free_command_sequence_arr(commands_arr, sz);
    }

}
