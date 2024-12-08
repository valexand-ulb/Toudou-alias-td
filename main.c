#include <errno.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Macro for debug message
#define info(msg, ...) printf("[i] " msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf("[!] " msg "\n", ##__VA_ARGS__)
#define okay(msg, ...) printf("[v] " msg "\n", ##__VA_ARGS__)

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
    if (argc == commands[i].argc_required &&
        strcmp(argv[1], commands[i].command) == 0) {
      printf("%s\n", commands[i].message); // TODO : modify this with
                                           // corresponding function ptr
      return 1;
    }
  }
  return 0;
}

#define DATABASE_NAME "events.db"
#define DATABASE_PATH "~/.local/share/todo/"
sqlite3 *database;

int expand_tilde(const char *path, char *expanded_path, size_t size) {
  /**
   * Replace ~ in a path by the HOME folder using environement variable
   * path : path to expand
   * expanded_path : pointer to path expanded
   * size : size of path to be expanded
   **/
  if (path[0] == '~') {
    const char *home = getenv("HOME");
    if (home) {
      snprintf(expanded_path, size, "%s%s", home, path + 1);
      return 1;
    }
    fprintf(stderr, "Error: Unable to resolve '~'.\n");
    return 0;
  }
  strncpy(expanded_path, path, size);
  return 0;
}

int initialize_database(const char *database_path, const char *database_name) {
  char expanded_path[256];
  char fullpath[512];
  int rc;

  // Expand the tilde in the database path
  if (!expand_tilde(database_path, expanded_path, sizeof(expanded_path))) {
    fprintf(stderr, "Error expanding path: %s\n", database_path);
    return 0;
  }

  // Construct full path to the database file
  if (snprintf(fullpath, sizeof(fullpath), "%s%s", expanded_path,
               database_name) >= sizeof(fullpath)) {
    fprintf(stderr, "Path too long: %s%s\n", expanded_path, database_name);
    return 0;
  }

  // Create the directory if it doesn't exist
  if (mkdir(expanded_path, 0755) < 0 && errno != EEXIST) {
    perror("Error creating directory");
    return 0;
  }

  // Open the SQLite database
  rc = sqlite3_open(fullpath, &database);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to open database '%s': %s\n", database_name,
            sqlite3_errmsg(database));
    sqlite3_close(database); // Ensure proper cleanup
    return 0;
  }

  printf("Database '%s' initialized successfully at: %s\n", database_name,
         fullpath);
  return 1;
}

int main(int argc, char **argv) {
  okay("this is okay message");
  warn("this is warning message");
  info("this is info message");

  if (!initialize_database(DATABASE_PATH, DATABASE_NAME)) {
    fprintf(stderr, "Error in database initialisation...\n");
    exit(1);
  }

  if (!check_arguments(argc, argv)) {
    fprintf(stderr, "Error in argument parsing...\n");
    exit(1);
  }
}