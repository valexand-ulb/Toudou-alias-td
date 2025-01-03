#include <stdlib.h>
#include <string.h>

#include "constant.h"
#include "database.h"
#include "log.h"
#include "utils.h"

size_t TABLE_SIZE = 0;

// Define a structure for mapping commands to actions
typedef struct
{
    const char* command;
    int argc_required;
    int (*predicate)(int, char**); // function attributed to an argument
    const char* error_message;
} Command;

int function_list(const int argc, char** argv)
{
    todo_type todo_list[TABLE_SIZE] = {};
    if (argc == 2)
    {
        info("function_list with default arg");
        char string[MAX_BUFFER_SIZE * MAX_PRINT_LINE] = "There is no todo left\n\0";
        fetch_todos(TABLE_SIZE, todo_list);
        format_string(MAX_PRINT_LINE, todo_list, string);
        printf("%s", string);
    }
    else if (argc == 3)
    {
        info("function_list with int arg");
        int maxline = atoi(argv[2]);
        if (!maxline || maxline < 0)
        {
            err("argument is not an unsigned int");
            return 1;
        }
        if (maxline > get_table_size())
        {
            err("The number of todo asked is too large, defaulting to 3");
            maxline = 3;
        }
        char string[MAX_BUFFER_SIZE * maxline];
        fetch_todos(TABLE_SIZE, todo_list);
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
    info("Remove task with id %u", id);
    if (id < 1 || id > TABLE_SIZE)
    {
        err("The id is invalid");
        return 1;
    }
    if (remove_todo(id))
    {
        warn("todo not removed");
        printf("No todo affected\n");
    }
    else
    {
        info("Removed task");
        printf("todo of id '%d' removed\n", id);
        TABLE_SIZE--;

        // fetch all todos to reorder them
        todo_type todo_list[TABLE_SIZE] = {}; //
        fetch_todos(TABLE_SIZE, todo_list);
        rearrange_todo(TABLE_SIZE, todo_list);
    }

    function_list(2, argv);
    return 0;
}

int function_time(int argc, char** argv)
{
    info("Modify time of a task");
    todo_type todo_list[TABLE_SIZE] = {};
    fetch_todos(TABLE_SIZE, todo_list);

    const unsigned id = atoi(argv[2]);
    if (!id || id > TABLE_SIZE)
    {
        err("The id is invalid");
        return 1;
    }
    const todo_type todo = todo_list[id - 1]; // id-1 since id on term start from 1
    time_t timestamp = todo.timestamp;
    info("got todo of time %s", strtok(ctime(&todo.timestamp), "\n"));
    manage_time_arg(argv[3], &timestamp);
    info("Updated timestamp %s", strtok(ctime(&timestamp), "\n"));
    update_time(id, timestamp);
    function_list(2, argv);
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
    {"time", 4, function_time, ERROR_TIME},
    {"debug_fill", 3, function_debug_fill, ERROR_FILL},
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

int check_arguments(const int argc, char** argv)
{
    int command_failed = -1;
    if (argc == 1)
    {
        err("Missing command");
        printf("Missing command\n");
        return 0;
    }
    for (size_t i = 0; i < COMMAND_COUNT; ++i)
    {
        if (strcmp(argv[1], commands[i].command) == 0)
        {
            if (argc == commands[i].argc_required)
            {
                okay("Command %s with correct parameters", commands[i].command);
                if (commands[i].predicate(argc, argv))
                    command_failed = i;
            }
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

int main(const int argc, char** argv)
{
    if (!initialize_database(DATABASE_PATH, DATABASE_NAME))
    {
        err("Error in database initialisation...");
        exit(1);
    }

    TABLE_SIZE = get_table_size();
    info("table size: %d", TABLE_SIZE);

    if (!check_arguments(argc, argv))
    {
        err("Error program Execution...");
        exit(1);
    }
    close_database();
}
