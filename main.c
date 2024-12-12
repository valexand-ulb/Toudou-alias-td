#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "log.h"


// Define type for different predicate signature
typedef int (*PredicateTypeListDefault)(void);
typedef int (*PredicateTypeListInt)(int);
typedef int (*PredicateTypeRemoveDefault)(int);

// Define a structure for mapping commands to actions
typedef struct
{
    const char* command;
    int argc_required;
    void *predicate; // function attributed to an argument
    int predicate_type;
} Command;

int function_list_default()
{
    info("function_list_default");
    char string[MAX_BUFFER_SIZE*MAX_PRINT_LINE];
    fetch_first_n_todos(MAX_PRINT_LINE, string);
    printf("%s", string);

    return 0;
}

int function_list_int(int number_of_line)
{
    info("function_list_int");
    char string[MAX_BUFFER_SIZE*number_of_line];
    fetch_first_n_todos(number_of_line, string);
    printf("%s", string);
    return 0;
}

int function_add()
{
    info("Add task");
    return 0;
}

int function_done(int todo_id)
{
    info("Remove task");
    return 0;
}

int function_time()
{
    info("Modify time of a task");
    return 0;
}

// Array of supported commands
static const Command commands[] = {
    {"list", 2, function_list_default, 1},
    {"list", 3, function_list_int, 2},
    {"add", 4, function_add},
    {"done", 3, function_done, 3},
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

    //_debug_fill_database();

    if (!check_arguments(argc, argv))
    {
        err("Error program Execution...");
        exit(1);
    }

    close_database();
}
