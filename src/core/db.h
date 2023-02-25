#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "config.h"

// our defualt values
#define MAX_QUERY_SIZE 512
#define DB_NAME "data.db"
#define INT_DB "INT"
#define STRING_DB "STRING"

// a struct for passing arguments to check table exists callback
struct Create_Callback_Data
{
    char *table_name;
    int result;
};
typedef struct Create_Callback_Data Create_Callback_Data;

// creates a name for a table appending the prefix to its name
void create_table_name(char *table_name, Token type, char *new_table_name)
{
    if (type == INT)
    {
        strcpy(new_table_name, strcat(table_name, INT_KEY));
    }
    else
    {
        strcpy(new_table_name, strcat(table_name, STRING_KEY));
    }
}

// get a data base handler it should be freed with free_db_handler
sqlite3 *get_db_handler()
{
    sqlite3 *db;
    sqlite3_open(DB_NAME, &db);
    return db;
}

// frees the database handler
void free_db_hanlder(sqlite3 *db)
{
    sqlite3_close(db);
}

// a call back for check table exists
int check_table_exists_callback(void *pass_data, int column_num, char **values, char **column_names)
{
    Create_Callback_Data *data = (Create_Callback_Data *)pass_data;
    for (int i = 0; i < column_num; i++)
    {
        if (!strcmp(values[i], data->table_name))
        {
            data->result = 1;
            return 0;
        }
    }
    return 0;
}

// checks whether a database table exists
int check_table_exists(char *table_name, sqlite3 *db)
{
    int status;
    char sql[MAX_QUERY_SIZE];
    char *err_msg;
    Create_Callback_Data *data = (Create_Callback_Data *)malloc(sizeof(Create_Callback_Data));
    data->table_name = table_name;
    data->result = 0;
    snprintf(sql, MAX_QUERY_SIZE, "SELECT name FROM sqlite_master"
                                  " WHERE type='table' AND name='%s';",
             table_name);
    status = sqlite3_exec(db, sql, check_table_exists_callback, (void *)data, &err_msg);
    if (status)
    {
        printf("something went wrong!\n error message:%s", err_msg);
        exit(0);
    }
    status = data->result;
    free(data);
    sqlite3_free(err_msg);
    return status;
}

// creates a table
void create_table(const char *name, const char *type, sqlite3 *db)
{
    int status;
    char *err_msg;
    char sql[MAX_QUERY_SIZE];
    snprintf(sql, MAX_QUERY_SIZE, "CREATE TABLE IF NOT EXISTS %s("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "msg %s NOT NULL,"
                                  "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP);",
             name, type);
    status = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (status)
    {
        printf("something went wrong!\n error message:%s", err_msg);
        exit(0);
    }
    sqlite3_free(err_msg);
}

// inserts a row in a give table
void insert(const char *table_name, const char *value, sqlite3 *db)
{
    int status;
    char *err_msg;
    char sql[MAX_QUERY_SIZE];
    snprintf(sql, MAX_QUERY_SIZE, "INSERT INTO %s(msg)"
                                  " VALUES('%s');",
             table_name, value);
    status = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (status)
    {
        printf("something went wrong!\n error message:%s", err_msg);
        exit(0);
    }
    sqlite3_free(err_msg);
}

// a call back for get_all_msg_sorted
int get_all_msg_sorted_callback(void *pass_data, int column_num, char **values, char **column_names)
{
    for (int i = 0; i < column_num; i++)
    {
        printf("%s = %s\n", column_names[i], values[i]);
    }
    printf("____________________________________\n");
    return 0;
}

// gets all the messages sorted by msg field
void get_all_msg_sorted(const char *table_name, sqlite3 *db)
{
    int status;
    char *err_msg;
    char sql[MAX_QUERY_SIZE];
    snprintf(sql, MAX_QUERY_SIZE, "SELECT * FROM %s "
                                  "ORDER BY msg ASC;",
             table_name);
    status = sqlite3_exec(db, sql, get_all_msg_sorted_callback, NULL, &err_msg);
    if (status)
    {
        printf("something went wrong!\n error message:%s", err_msg);
        exit(0);
    }
    sqlite3_free(err_msg);
}