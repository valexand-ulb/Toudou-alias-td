#include <stdio.h>
#include <string.h>

// Define a structure for mapping commands to actions
typedef struct {
    const char *command;
    int argc_required;
    const char *message;
} Command;

// Array of supported commands
static const Command commands[] = {
    {"list", 2, "List task"},
    {"add", 3, "Add task"},
    {"done", 3, "Remove task"},
    {"time", 3, "Modify time of a task"},
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

int check_arguments(int argc, char **argv) {
    for (size_t i = 0; i < COMMAND_COUNT; ++i) {
        if (argc == commands[i].argc_required && strcmp(argv[1], commands[i].command) == 0) {
            printf("%s\n", commands[i].message); // TODO : modify this with corresponding function ptr
            return 1;
        }
    }
    return 0;
}


int main(int argc, char **argv)
{
    if (!check_arguments(argc, argv)) {
        printf("Error in argument parsing...\n");
    }
}