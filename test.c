#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

int main()
{
    sqlite3 *db;
    char *errMsg = 0;
    int input;

    input = sqlite3_open("base.db", &db);
    if (input)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    input = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, age INTEGER)", NULL, NULL, &errMsg);

    if (input != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }

    input = sqlite3_exec(db, "INSERT INTO users (name, age) VALUES ('John', 30), ('Dee', 25), ('Bob', 44)", NULL, NULL, &errMsg);
    if (input != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }

    // Query the database
    sqlite3_stmt *statement;
    input = sqlite3_prepare_v2(db, "SELECT * FROM users", -1, &statement, NULL);
    if (input != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    printf("Users:\n");
    while ((input = sqlite3_step(statement)) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(statement, 0);
        const char *name = (const char *)sqlite3_column_text(statement, 1);
        int age = sqlite3_column_int(statement, 2);
        printf("ID: %d, Name: %s, Age: %d\n", id, name, age);
    }

    if (input != SQLITE_DONE)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);

    return 0;

}