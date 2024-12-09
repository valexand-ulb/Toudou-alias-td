//
// Created by alex on 9/12/24.
//

#ifndef DATABASE_H
#define DATABASE_H

#define DATABASE_NAME "events.db"
#define DATABASE_PATH "~/.local/share/todo/"

int initialize_database();
int fetch_first_n_todos(int max_line, char **array);


#endif //DATABASE_H
