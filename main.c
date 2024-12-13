#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "log.h"


// Define a structure for mapping commands to actions
typedef struct
{
    const char* command;
    int argc_required;
    int (*predicate)(int, char**); // function attributed to an argument
    int predicate_type;
} Command;

int function_list(int argc, char** argv)
{
    if (argc == 2)
    {
        info("function_list with default arg");
        char string[MAX_BUFFER_SIZE * MAX_PRINT_LINE] = "There is no todo left\0";
        fetch_first_n_todos(MAX_PRINT_LINE, string);
        printf("%s", string);
    }
    else if (argc == 3)
    {
        info("function_list with int arg");
        int maxline = atoi(argv[2]);
        if (maxline > get_size_of_table())
        {
            err("The number of todo asked is too large, defaulting to 3");
            maxline = 3;
        }
        char string[MAX_BUFFER_SIZE * maxline];
        fetch_first_n_todos(maxline, string);
        printf("%s", string);
    }

    return 0;
}

int function_add(int argc, char** argv)
{
    info("Add task");
    return 0;
}

int function_done(int argc, char** argv)
{
    info("Remove task");
    const unsigned id = atoi(argv[2]);
    if (!remove_todo(id))
    {
        warn("todo not removed");
        close_database();
        exit(1);
    }
    printf("todo of id '%d' removed\n", id);
    function_list(2, argv);
    return 0;
}

int function_time(int argc, char** argv)
{
    info("Modify time of a task");
    return 0;
}

// Array of supported commands
static const Command commands[] = {
    {"list", 2, function_list},
    {"list", 3, function_list},
    {"add", 4, function_add},
    {"done", 3, function_done},
    {"time", 3, function_time},
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

int check_arguments(int argc, char** argv)
{
    for (size_t i = 0; i < COMMAND_COUNT; ++i)
    {
        if (argc == commands[i].argc_required &&
            strcmp(argv[1], commands[i].command) == 0)
        {
            // function execution here
            commands[i].predicate(argc, argv);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    if (!initialize_database(DATABASE_PATH, DATABASE_NAME))
    {
        err("Error in database initialisation...");
        exit(1);
    }

    int size = get_size_of_table();
    printf("Table size: %d\n", size);

    //_debug_fill_database();

    if (!check_arguments(argc, argv))
    {
        err("Error program Execution...");
        exit(1);
    }

    close_database();
}
