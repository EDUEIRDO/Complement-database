#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

void randomNames()
{
    
}

int insert(char name[], int age)
{
    sqlite3 *db;
    char *errMsg = 0;
    int entry;

    entry = sqlite3_open("base.db", &db);
    if (entry)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    //Insert personalized
    sqlite3_stmt *insertStatement;
    entry = sqlite3_prepare_v2(db, "INSERT INTO users (name, age) VALUES (?, ?)", -1, &insertStatement, NULL);
    if (entry == SQLITE_OK)
    {
        sqlite3_bind_text(insertStatement, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_int(insertStatement, 2, age);
        if (sqlite3_step(insertStatement) != SQLITE_DONE)
        {
            fprintf(stderr, "Insert operation failed: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(insertStatement);
}

int update(char name[], int age)
{
    sqlite3 *db;
    char *errMsg = 0;
    int entry;

    entry = sqlite3_open("base.db", &db);
    if (entry)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sqlite3_stmt *updateStatement;
    entry = sqlite3_prepare_v2(db, "UPDATE users SET name = ?, age = ? WHERE id = 1;", -1, &updateStatement, NULL);
    if (entry == SQLITE_OK)
    {
        sqlite3_bind_text(updateStatement, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_int(updateStatement, 2, age);
        if (sqlite3_step(updateStatement) != SQLITE_DONE)
        {
            fprintf(stderr, "Update operation failed: %s\n", sqlite3_errmsg(db));
        }
    }

}
int delete(int id)
{
    sqlite3 *db;
    char *errMsg = 0;
    int entry;

    entry = sqlite3_open("base.db", &db);
    if (entry)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sqlite3_stmt *updateStatement;
    entry = sqlite3_prepare_v2(db, "DELETE FROM users WHERE id = ?;", -1, &updateStatement, NULL);
    if (entry == SQLITE_OK)
    {
        sqlite3_bind_int(updateStatement, 1, id);
        if (sqlite3_step(updateStatement) != SQLITE_DONE)
        {
            fprintf(stderr, "Delete operation failed: %s\n", sqlite3_errmsg(db));
        }
    }

}

int main()
{

    sqlite3 *db;
    char *errMsg = 0;
    int input;
    // char name[50];
    // int age;
    // // Ask user for their name
    // printf("Enter your name: ");
    // // Use fgets for safety, but trim newline
    // if (fgets(name, sizeof(name), stdin)) {
    //     size_t len = strlen(name);
    //     if (len > 0 && name[len-1] == '\n') {
    //         name[len-1] = '\0';
    //     }
    // }

    // printf("Enter your age: ");
    // scanf("%d", &age);

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

    // input = sqlite3_exec(db, "INSERT INTO users (name, age) VALUES ('John', 30), ('Dee', 25), ('Bob', 44)", NULL, NULL, &errMsg);
    
    // if (input != SQLITE_OK)
    // {
    //     fprintf(stderr, "SQL error: %s\n", errMsg);
    //     sqlite3_free(errMsg);
    //     return 1;
    // }

    //Insert personalized
    // sqlite3_stmt *insertStatement;
    // input = sqlite3_prepare_v2(db, "INSERT INTO users (name, age) VALUES (?, ?)", -1, &insertStatement, NULL);
    // if (input == SQLITE_OK)
    // {
    //     sqlite3_bind_text(insertStatement, 1, name, -1, SQLITE_STATIC);
    //     sqlite3_bind_int(insertStatement, 2, age);
    //     if (sqlite3_step(insertStatement) != SQLITE_DONE)
    //     {
    //         fprintf(stderr, "Insert failed: %s\n", sqlite3_errmsg(db));
    //     }
    // }
    // sqlite3_finalize(insertStatement);
    char entry[100];
    printf("Digite seu nome:");
    fgets(entry, sizeof(entry), stdin);
    entry[strcspn(entry, "\n")] = '\0';

    //insert(entry, 20);
    update(entry, 21);
    delete(1);


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