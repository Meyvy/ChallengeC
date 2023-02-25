#pragma once
#include "db.h"
#include "config.h"
#include <unistd.h>

// our wait duration
#define CHECK_DURATION 2 * 60

// every 2 minutes it prints all the rows of the tables with int messages in ascending order
void monitor(Config *config)
{
    sqlite3 *db = get_db_handler();
    while (1)
    {
        sleep(CHECK_DURATION);
        for (int i = 0; i < config->table_size; i++)
        {
            if (config->tables[i]->type == STRING)
            {
                continue;
            }
            printf("***********\n");
            printf("beginning of table %s\n", config->tables[i]->table_name);
            get_all_msg_sorted(config->tables[i]->table_name, db);
            printf("end of table %s\n", config->tables[i]->table_name);
            printf("***********\n");
        }
    }
}