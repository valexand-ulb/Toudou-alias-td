#include <errno.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "database.h"

#include "log.h"

sqlite3 *database = NULL;

int expand_tilde(const char *path, char *expanded_path, size_t size)
{
  /**
   * Replace ~ in a path by the HOME folder using environement variable
   * path : path to expand
   * expanded_path : pointer to path expanded
   * size : size of path to be expanded
   **/
  if (path[0] == '~')
  {
    const char *home = getenv("HOME");
    if (home)
    {
      snprintf(expanded_path, size, "%s%s", home, path + 1);
      return 1;
    }
    err("Error : Unable to resolve '~'.");
    return 0;
  }
  strncpy(expanded_path, path, size);
  return 0;
}

int create_database_table(char * err_msg) {
  const char *sql =
      "CREATE TABLE IF NOT EXISTS todos ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "content TEXT NOT NULL, "
      "datetime DATETIME NOT NULL);";

  // Execute the SQL command
  int rc = sqlite3_exec(database, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    err("Failed to initialize database '%s': %s", DATABASE_NAME,
            err_msg);
    sqlite3_free(err_msg); // Free error message memory
    sqlite3_close(database);
    return 0;
  }
  return 1;
}

int fetch_first_n_todos(int max_line, char **array) {
  // TODO ; Not working SQL error 21 -> type mismatch
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT id, content, datetime FROM todos ORDER BY id LIMIT ?;";
  // prepare sql statement
  int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    err("Failed to prepare statement: %s\n", sqlite3_errmsg(database));
    return 0;
  }
  info("Fetching the first %d Todos by ID:\n", max_line);

  // assing
  rc = sqlite3_bind_int(stmt, 1, max_line);
  if (rc != SQLITE_OK) {
    err( "Failed to bind limit: %s\n", sqlite3_errmsg(database));
    sqlite3_finalize(stmt);
    return 0;
  }

  return 1;
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
  if (snprintf(fullpath, sizeof(fullpath), "%s%s", expanded_path, DATABASE_NAME) >= sizeof(fullpath)) {
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
    err("Error opening database at '%s': %s", fullpath, sqlite3_errmsg(database));
    return 0;
  }

  rc = create_database_table(err_msg);

  // Success message
  info("Database '%s' initialized successfully at: %s", DATABASE_NAME, fullpath);

  // Close the database connection
  sqlite3_close(database);
  return 1;
}