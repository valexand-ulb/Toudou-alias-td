#ifndef DATABASE_H
#define DATABASE_H

#define DATABASE_NAME "events.db"
#define DATABASE_PATH "~/.local/share/todo/"

#define MAX_PRINT_LINE 3
#define MAX_BUFFER_SIZE 256

#include <stddef.h>
#include <time.h>

// GLOBAL VARIABLE
typedef struct
{
    int id;
    char content[256];
    time_t timestamp;
} todo_type;

// BASE STATEMENT FUNCTIONS
int initialize_database();

void _debug_fill_database(int num);

int close_database();

// SIMPLE SQL REQUEST
int get_table_size();

int add_todo(const todo_type *todo);

int remove_todo(const unsigned todo_id);

int rearrange_todo(int table_size, todo_type todo_list[]);

int updtate_sequence_number(int new_sequence_number);
int update_time(const unsigned id, const long int newtimestamp);


// COMPOSED SQL REQUEST

int fetch_todos(const size_t table_size, todo_type todo_list[]);

// OTHERS
int format_string(int line_to_print, todo_type todo_list[], char* string);

#endif  // DATABASE_H
