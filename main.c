#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "log.h"

#define MAX_PRINT_LINE 3

// Define a structure for mapping commands to actions
typedef struct {
  const char *command;
  int argc_required;
  int (*predicate)(void);  // function attributed to an argument
} Command;

int function_list() {
  info("List task");
  char *array[256];
  fetch_first_n_todos(10,array);
  return 0;
}

int function_add() {
  info("Add task");
  return 0;
}

int function_done() {
  info("Remove taks");
  return 0;
}

int function_time() {
  info("Modify time of a task");
  return 0;
}

// Array of supported commands
static const Command commands[] = {
    {"list", 2, function_list},
    {"add", 4, function_add},
    {"done", 3, function_done},
    {"time", 3, function_time},
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

int check_arguments(int argc, char **argv) {
  for (size_t i = 0; i < COMMAND_COUNT; ++i) {
    if (argc == commands[i].argc_required &&
        strcmp(argv[1], commands[i].command) == 0) {
      commands[i].predicate();
      return 1;
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  if (!initialize_database(DATABASE_PATH, DATABASE_NAME)) {
    err("Error in database initialisation...");
    exit(1);
  }

  //_debug_fill_database();

  if (!check_arguments(argc, argv)) {
    err("Error program Execution...");
    exit(1);
  }

  close_database();
}