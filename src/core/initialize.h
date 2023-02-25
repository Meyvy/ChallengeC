#pragma once
#include "db.h"
#include "config.h"
#include "monitor.h"
#include "server.h"

// sets up the database creating tables if they do not exist
void set_up_db(Config *config)
{
    sqlite3 *db = get_db_handler();
    char table_name[MAX_DATABASE_NAME_SIZE];
    bzero(table_name, MAX_DATABASE_NAME_SIZE);
    for (int i = 0; i < config->table_size; i++)
    {
        Table *table = config->tables[i];
        create_table_name(table->table_name, table->type, table_name);
        if (table->type == INT)
        {
            create_table(table_name, INT_DB, db);
        }
        else
        {
            create_table(table_name, STRING_DB, db);
        }
        bzero(table_name, MAX_DATABASE_NAME_SIZE);
    }
    free_db_hanlder(db);
}

// our main entry point
void start()
{
    Config *config = parse_config();
    set_up_db(config);

    // spawn a new process to handle the monitoring
    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        monitor(config);
        exit(0);
    }

    // listen on the defined port
    listen_and_serve(config->port);
}