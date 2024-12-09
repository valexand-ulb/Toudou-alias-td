#include "database.h"

#include <errno.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "log.h"


sqlite3 *database = NULL;


typedef struct {
  int id;
  const char *content;
  const char *datetime;
} todo_type;

// SQL UNRELATED

int expand_tilde(const char *path, char *expanded_path, size_t size) {
  if (path[0] == '~') {
    const char *home = getenv("HOME");
    if (home) {
      snprintf(expanded_path, size, "%s%s", home, path + 1);
      return 1;
    }
    err("Error : Unable to resolve '~'.");
    return 0;
  }
  strncpy(expanded_path, path, size);
  return 0;
}

// BASE STATEMENT FUNCTIONS

sqlite3_stmt *prepare_statement(const char *sql) {
  sqlite3_stmt *stmt = NULL;
  int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    err("Failed to prepare statement: %s", sqlite3_errmsg(database));
    sqlite3_close(database);
    return NULL;
  }
  return stmt;
}

int execute_statement(sqlite3_stmt *stmt, char *command) {
  int rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    err("Failed to execute statement: %s", sqlite3_errmsg(database));
  } else {
    info("'%s' successfull", command);
  }
  return rc;
}

// BASE DATABASE FUNCTION

int create_database_table() {
  const char *sql =
      "CREATE TABLE IF NOT EXISTS todos ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "content TEXT NOT NULL, "
      "datetime INTEGER NOT NULL);";

  sqlite3_stmt *stmt = prepare_statement(sql);
  int rc = execute_statement(stmt, "create_database_table");
  sqlite3_finalize(stmt);
  return  rc == SQLITE_DONE;
}

int initialize_database() {
  char expanded_path[256];
  char fullpath[512];
  char *err_msg = NULL;

  // Expand the tilde in the database path
  if (!expand_tilde(DATABASE_PATH, expanded_path, sizeof(expanded_path))) {
    err("Error expanding path: %s", DATABASE_PATH);
    return 0;
  }

  // Construct the full path to the database file
  if (snprintf(fullpath, sizeof(fullpath), "%s%s", expanded_path,
               DATABASE_NAME) >= sizeof(fullpath)) {
    err("Path too long: %s%s", expanded_path, DATABASE_NAME);
    return 0;
               }

  // Create the directory if it doesn't exist
  if (mkdir(expanded_path, 0755) < 0 && errno != EEXIST) {
    perror("Error creating directory");
    return 0;
  }

  // Open the SQLite database
  int rc = sqlite3_open(fullpath, &database);
  if (rc != SQLITE_OK) {
    err("Error opening database at '%s': %s", fullpath,
        sqlite3_errmsg(database));
    return 0;
  }

  create_database_table(err_msg);

  // Success message
  info("Database '%s' initialized successfully at: %s", DATABASE_NAME,
       fullpath);

  // Close the database connection

  return 1;
}

int close_database() { return sqlite3_close(database); }

void _debug_fill_database() {
  for (unsigned i = 0; i < 10; ++i) {
    char event[128];
    snprintf(event, sizeof(event), "debug_text_%d", i);
    add_todo(event, (int64_t)time(NULL));
  }
}

// SIMPLE SQL REQUEST

int add_todo(const char *content, int64_t timestamp) {
  const char *sql =
      "INSERT INTO todos (content, datetime) VALUES (?, "
      "datetime(?, 'unixepoch'));";
  ;

  // Prepare the SQL statement
  sqlite3_stmt *stmt = prepare_statement(sql);
  if (!stmt) return 0;

  // Bind the parameters
  sqlite3_bind_text(stmt, 1, content, -1, SQLITE_STATIC);  // Bind the content
  sqlite3_bind_int64(stmt, 2, timestamp);

  // Execute the statement
  int rc = execute_statement(stmt, "add_todo");

  // Finalize the statement and close the database
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

int fetch_todo(sqlite3_stmt *stmt, todo_type *todo_list) {
  int rc = 0;
  // Execute the statement and process results (example, not implemented fully
  // here)
  unsigned i = 0;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    todo_list[i].id = sqlite3_column_int(stmt, 0);
    todo_list[i].content  = (const char *)sqlite3_column_text(stmt, 1);
    todo_list[i].datetime = (const char *)sqlite3_column_text(stmt, 2);

    // Process each row (store in `array` or any other desired processing)
    info("Todo %d: %s at %s",
      todo_list[i].id,
      todo_list[i].content,
      todo_list[i].datetime);
    ++i;
  }
  return rc;
}

// COMPOSED SQL REQUEST

int fetch_first_n_todos(int max_line, char **array) {
  char sql[128];
  todo_type todo_list[max_line];

  snprintf(sql, sizeof(sql),
           "SELECT id, content, datetime FROM todos ORDER BY id LIMIT %d;",
           max_line);

  // Prepare the SQL statement
  sqlite3_stmt *stmt = prepare_statement(sql);

  int rc = fetch_todo(stmt, todo_list);

  if (rc != SQLITE_DONE) {
    err("Error executing query: %s\n", sqlite3_errmsg(database));
  }

  // Finalize the statement
  sqlite3_finalize(stmt);

  return rc == SQLITE_DONE;
}



