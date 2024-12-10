//
// Created by alex on 9/12/24.
//

#ifndef DATABASE_H
#define DATABASE_H

#define DATABASE_NAME "events.db"
#define DATABASE_PATH "~/.local/share/todo/"

#include <stdint.h>

void _debug_fill_database();
int add_todo(const char *event, int64_t timestamp);
int initialize_database();
int fetch_first_n_todos(int max_line, char **array);
int close_database();

#endif  // DATABASE_H
