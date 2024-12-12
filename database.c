#include "database.h"

#include <errno.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "log.h"
#include "utils.h"


sqlite3* database = NULL;


typedef struct
{
    int id;
    char content[256];
    time_t timestamp;
} todo_type;



// BASE STATEMENT FUNCTIONS

sqlite3_stmt* prepare_statement(const char* sql)
{
    sqlite3_stmt* stmt = NULL;
    int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        err("Failed to prepare statement: %s", sqlite3_errmsg(database));
        sqlite3_close(database);
        return NULL;
    }
    return stmt;
}

int execute_statement(sqlite3_stmt* stmt, char* command)
{

    const int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        err("Failed to execute statement: %s", sqlite3_errmsg(database));
    }
    else
    {
        info("'%s' successfull", command);
    }
    return rc;
}

// BASE DATABASE FUNCTION

int create_database_table()
{
    const char* sql_table =
        "CREATE TABLE IF NOT EXISTS todos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "content TEXT NOT NULL, "
        "datetime INTEGER NOT NULL);";

    // Optimized trigger to reset IDs from 1 to n aft

    sqlite3_stmt* stmt = NULL;
    int rc = SQLITE_OK;

    // Prepare and execute the table creation statement
    stmt = prepare_statement(sql_table);

    rc = execute_statement(stmt, "create_database_table");

    sqlite3_finalize(stmt);

    return SQLITE_OK;
}

int initialize_database()
{
    char expanded_path[256];
    char fullpath[512];
    char* err_msg = NULL;

    // Expand the tilde in the database path
    if (!expand_tilde(DATABASE_PATH, expanded_path, sizeof(expanded_path)))
    {
        err("Error expanding path: %s", DATABASE_PATH);
        return 0;
    }

    // Construct the full path to the database file
    if (snprintf(fullpath, sizeof(fullpath), "%s%s", expanded_path,
                 DATABASE_NAME) >= sizeof(fullpath))
    {
        err("Path too long: %s%s", expanded_path, DATABASE_NAME);
        return 0;
    }

    // Create the directory if it doesn't exist
    if (mkdir(expanded_path, 0755) < 0 && errno != EEXIST)
    {
        perror("Error creating directory");
        return 0;
    }

    // Open the SQLite database
    int rc = sqlite3_open(fullpath, &database);
    if (rc != SQLITE_OK)
    {
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

void _debug_fill_database()
{
    for (unsigned i = 0; i < 10; ++i)
    {
        char event[128];
        snprintf(event, sizeof(event), "debug_text_%d", i);
        add_todo(event, (unsigned long)time(NULL));
    }
}

// SIMPLE SQL REQUEST

int add_todo(const char* content, const long long timestamp)
{
    const char* sql =
        "INSERT INTO todos (content, datetime) VALUES (?,?);";

    // Prepare the SQL statement
    sqlite3_stmt* stmt = prepare_statement(sql);
    if (!stmt) return 0;

    if (sqlite3_bind_text(stmt, 1, content, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        err("Failed to bind content: %s\n", sqlite3_errmsg(database));
        sqlite3_finalize(stmt);
        return 1;
    }
    if (sqlite3_bind_int64(stmt, 2, timestamp) != SQLITE_OK)
    {
        err("Failed to bind datetime: %s\n", sqlite3_errmsg(database));
        sqlite3_finalize(stmt);
        return 1;
    }

    // Execute the statement
    const int rc = execute_statement(stmt, "add_todo");

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

int remove_todo(const unsigned todo_id)
{
    const char* sql =
        "DELETE FROM todos WHERE id = ?";

    sqlite3_stmt *stmt = prepare_statement(sql);

    if (sqlite3_bind_int(stmt, 1, todo_id) != SQLITE_OK)
        {
        err("Failed to bind id: %s\n", sqlite3_errmsg(database));
        sqlite3_finalize(stmt);
        return 1;
    }

    // Execute the statement
    const int rc = execute_statement(stmt, "remove_todo");
    if (rc != SQLITE_DONE)
    {
        err("Failed to execute delete statement: %s\n", sqlite3_errmsg(database));
        sqlite3_finalize(stmt);
        return 1;
    }

    // Check if a row was deleted
    const int changes = sqlite3_changes(database);
    sqlite3_finalize(stmt);

    if (changes == 0)
    {
        warn("no row has been deleted");
        return 0;
    }
    return rc;
}

int fetch_todo(sqlite3_stmt* stmt, todo_type todo_list[])
{
    int rc = 0;
    unsigned i = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        todo_list[i].id = sqlite3_column_int(stmt, 0);

        strncpy(todo_list[i].content, (const char *)sqlite3_column_text(stmt, 1), sizeof(todo_list[i].content) - 1);
        todo_list[i].content[sizeof(todo_list[i].content) - 1] = '\0';

        todo_list[i].timestamp = (time_t) sqlite3_column_int64(stmt, 2);

        char print_str[32];
        timestamp_to_string(todo_list[i].timestamp, print_str, sizeof(print_str));

        info("Todo %d: %s at %s ",
             todo_list[i].id,
             todo_list[i].content,
             print_str);

        ++i;
    }
    return rc;
}

// COMPOSED SQL REQUEST

int fetch_first_n_todos(const int max_line, char *string)
{
    char sql[128];
    todo_type todo_list[max_line] = {};

    snprintf(sql, sizeof(sql),
             "SELECT id, content, datetime FROM todos ORDER BY id LIMIT %d;",
             max_line);

    // Prepare the SQL statement
    sqlite3_stmt* stmt = prepare_statement(sql);

    int rc = fetch_todo(stmt, todo_list);

    if (rc != SQLITE_DONE)
    {
        err("Error executing query: %s\n", sqlite3_errmsg(database));
    }

    sqlite3_finalize(stmt);
    for (unsigned i = 0; i < sizeof(todo_list) / sizeof(todo_type); ++i)
    {
        if (todo_list[i].timestamp) // if todo_element is not default
        {
            char formatted_todo[256];
            char datetime_str[32];

            timestamp_to_string(todo_list[i].timestamp, datetime_str, sizeof(datetime_str));

            snprintf(formatted_todo, sizeof(formatted_todo), "%d : %s - %s\n",
                     todo_list[i].id, todo_list[i].content, datetime_str);

            strncat(string, formatted_todo, max_line * sizeof(todo_list[0]) - strlen(string) - 1);
        }
    }

    return rc == SQLITE_DONE;
}
