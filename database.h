//
// Created by alex on 9/12/24.
//

#ifndef DATABASE_H
#define DATABASE_H

#define DATABASE_NAME "events.db"
#define DATABASE_PATH "~/.local/share/todo/"

#define MAX_PRINT_LINE 3
#define MAX_BUFFER_SIZE 256

#include <stddef.h>
#include <stdint.h>
// GLOBAL VARIABLE

// BASE STATEMENT FUNCTIONS
int initialize_database();
void _debug_fill_database();
int close_database();

// SIMPLE SQL REQUEST
int get_table_size();
int add_todo(const char* event, long long timestamp);

int remove_todo(const unsigned todo_id);

// COMPOSED SQL REQUEST

int fetch_todos(const size_t table_size, int max_line_printed, char *string);


#endif  // DATABASE_H
