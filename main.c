#include <stdlib.h>
#include <string.h>

#include "constant.h"
#include "database.h"
#include "log.h"
#include "utils.h"


int table_size = 0;
// Define a structure for mapping commands to actions
typedef struct
{
    const char* command;
    int argc_required;
    int (*predicate)(int, char**); // function attributed to an argument
    const char * error_message;
} Command;

int function_list(int argc, char** argv)
{
    todo_type todo_list[table_size] = {};
    if (argc == 2)
    {
        info("function_list with default arg");
        char string[MAX_BUFFER_SIZE * MAX_PRINT_LINE] = "There is no todo left\0";
        fetch_todos(table_size, todo_list);
        format_string(MAX_PRINT_LINE, todo_list, string);
        printf("%s", string);
    }
    else if (argc == 3)
    {
        info("function_list with int arg");
        int maxline = atoi(argv[2]);
        if (maxline > get_table_size())
        {
            err("The number of todo asked is too large, defaulting to 3");
            maxline = 3;
        }
        char string[MAX_BUFFER_SIZE * maxline];
        fetch_todos(table_size, todo_list);
        format_string(maxline, todo_list, string);
        printf("%s", string);
    }

    return 0;
}

int function_add(int argc, char** argv)
{
    info("Add task");
    todo_type todo;
    snprintf(todo.content, sizeof(todo.content), "%s", argv[2]);

    todo.timestamp = TIMESTAMP_NOW; // time will be added over current timestamp

    manage_time_arg(argv[3], &todo.timestamp);

    add_todo(&todo);
    return 0;
}

int function_done(int argc, char** argv)
{
    info("Remove task");

    // remove specific todos
    const unsigned id = atoi(argv[2]);
    if ( id > table_size || remove_todo(id))
    {
        warn("todo not removed");
        printf("No todo affected\n");
    } else
    {
        info("Removed task");
        printf("todo of id '%d' removed\n", id);
        table_size--;

        // fetch all todos to reorder them
        todo_type todo_list[table_size] = {}; //
        fetch_todos(table_size, todo_list);
        rearrange_todo(table_size, todo_list);
    }

    function_list(2, argv);
    return 0;
}

int function_time(int argc, char** argv)
{
    info("Modify time of a task");
    return 0;
}

int function_debug_fill(int argc, char** argv)
{
    info("Fill database task");
    _debug_fill_database(atoi(argv[2]));
    return 0;
}

// Array of supported commands
static const Command commands[] = {
    {"list", 2, function_list, ERROR_LIST},
    {"list", 3, function_list, ERROR_LIST},
    {"add", 4, function_add, ERROR_ADD},
    {"done", 3, function_done, ERROR_DONE},
    {"time", 3, function_time, ERROR_TIME},
    {"debug_fill", 3, function_debug_fill, ERROR_FILL},
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

int check_arguments(int argc, char** argv)
{
    int command_failed = -1;
    for (size_t i = 0; i < COMMAND_COUNT; ++i)
    {
        if (strcmp(argv[1], commands[i].command) == 0)
        {
            if (argc == commands[i].argc_required)
            {
                okay("Command %s with correct parameters", commands[i].command);
                commands[i].predicate(argc, argv);
                return 1;
            }
            else
                command_failed = i;
        }
    }
    if (command_failed > 0)
    {
        err("Command %s with invalid arguments", commands[command_failed].command);
        printf("Command %s with invalid arguments:\n", commands[command_failed].command);
        printf("%s\n", commands[command_failed].error_message);
        return 1;
    }
    err("Unrecognized command\n");
    return 0;
}

int main(int argc, char** argv)
{
    if (!initialize_database(DATABASE_PATH, DATABASE_NAME))
    {
        err("Error in database initialisation...");
        exit(1);
    }

    table_size = get_table_size();
    info("table size: %d\n", table_size);


    if (!check_arguments(argc, argv))
    {
        err("Error program Execution...");
        exit(1);
    }

    close_database();
}
