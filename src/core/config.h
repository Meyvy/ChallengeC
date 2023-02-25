#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// our defualt values
#define CONFIG_FILE_NAME "ini.fconfig"
#define MAX_LINE_SIZE 1024
#define MAX_DATABASE_NAME_SIZE 64
#define MAX_DATABASE_TABLE_NUM 100
#define PREFIX_TAG "[prefix]"
#define ADDRESS_TAG "[address]"
#define INT_KEY "p1"
#define STRING_KEY "p2"
#define PORT_KEY "port"
#define PREFIX_SIZE 2

// our tokens for parsing the config file
typedef enum Token
{
    PREFIX,
    ADDRESS,
    INT,
    STRING,
    PORT,
    INVALID
} Token;

// a struct holding table's information
struct Table
{
    char *table_name;
    Token type;
};
typedef struct Table Table;

// a struct holding all the config's information
struct Config
{
    Table **tables;
    int table_size;
    int port;
};
typedef struct Config Config;

// An initializor for Config struct instances. allocated memory should be freed with free config
void initilize_config(Config **config)
{
    *config = (Config *)malloc(sizeof(Config));
    (*config)->table_size = 0;
    (*config)->tables = (Table **)malloc(MAX_DATABASE_TABLE_NUM * sizeof(Table *));

    for (int i = 0; i < MAX_DATABASE_TABLE_NUM; i++)
    {
        (*config)->tables[i] = (Table *)malloc(sizeof(Table));
    }
    for (int i = 0; i < MAX_DATABASE_TABLE_NUM; i++)
    {
        (*config)->tables[i]->table_name = (char *)malloc(sizeof(char) * MAX_DATABASE_NAME_SIZE);
    }
}

// frees the allocated memory of Config struct instances
void free_config(Config *config)
{
    for (int i = 0; i < MAX_DATABASE_TABLE_NUM; i++)
    {
        free(config->tables[i]->table_name);
    }
    for (int i = 0; i < MAX_DATABASE_TABLE_NUM; i++)
    {
        free(config->tables[i]);
    }
    free(config);
}

// reads one line up to the line feed charachter. ignores the white space
int get_line(FILE *fp, char *line, int *size)
{
    char ch;
    int i = 0;
    do
    {
        ch = fgetc(fp);
        if (ch == ' ')
        {
            continue;
        }
        if (ch == '\n' || ch == EOF)
        {
            if (i == 0)
            {
                return 0;
            }
            *size = i;
            return 1;
        }
        line[i++] = ch;

        if (i >= MAX_LINE_SIZE)
        {
            printf("invalid config file: maximum line size exceeded");
            exit(0);
        }
    } while (ch != EOF);

    return 0;
}

// gets one line and returns the line's token
Token get_token(char *line, int line_size, char *value)
{

    // check for tag
    if (!strcmp(line, PREFIX_TAG))
    {
        return PREFIX;
    }
    if (!strcmp(line, ADDRESS_TAG))
    {
        return ADDRESS;
    }

    // check for key pair opetions
    char key[line_size];
    bzero(key, line_size);
    int i, j = 0;

    for (i = 0; i < line_size; i++)
    {
        if (line[i] == ' ')
        {
            continue;
        }
        if (line[i] == '=')
        {
            break;
        }
        key[j++] = line[i];
    }
    j = 0;
    for (i = i + 1; i < line_size; i++)
    {
        value[j++] = line[i];
    }

    if (!j)
    {
        printf("invalid key pair");
        exit(0);
    }
    if (!strcmp(key, INT_KEY))
    {
        return INT;
    }
    else if (!strcmp(key, STRING_KEY))
    {
        return STRING;
    }
    else if (!strcmp(key, PORT_KEY))
    {
        return PORT;
    }
    else
    {
        printf("invalid key pair");
        exit(0);
    }
}

// parses the config file and returns a Config pointer containing all the information. the pointer must be freed after use.
Config *parse_config()
{
    FILE *fp;
    fp = fopen("ini.fconfig", "r");
    if (fp == NULL)
    {
        printf("can't open the config file");
        exit(0);
    }

    char line[MAX_LINE_SIZE], value[MAX_DATABASE_NAME_SIZE];
    bzero(line, MAX_LINE_SIZE);
    bzero(value, MAX_DATABASE_NAME_SIZE);
    int *line_size = (int *)malloc(sizeof(int));
    Token token;
    Config *config;

    initilize_config(&config);

    if (!get_line(fp, line, line_size))
    {
        printf("invalid config:empty config");
        exit(0);
    }

    token = get_token(line, *line_size, value);
    if (token != ADDRESS)
    {
        printf("invalid config: no address tag");
        exit(0);
    }
    bzero(line, MAX_LINE_SIZE);

    if (!get_line(fp, line, line_size))
    {
        printf("invalid config: no address options");
        exit(0);
    }
    bzero(value, MAX_DATABASE_NAME_SIZE);

    token = get_token(line, *line_size, value);
    if (token != PORT)
    {
        printf("invalid config: invalid address option");
        exit(0);
    }
    config->port = atoi(value);
    if (config->port <= 0)
    {
        printf("invalid port number");
        exit(0);
    }
    bzero(line, MAX_LINE_SIZE);

    if (!get_line(fp, line, line_size))
    {
        printf("invalid config: no prefix tag");
        exit(0);
    }
    bzero(value, MAX_DATABASE_NAME_SIZE);

    token = get_token(line, *line_size, value);
    if (token != PREFIX)
    {
        printf("invalid config:  no prefix tag");
        exit(0);
    }
    while (get_line(fp, line, line_size))
    {
        if (config->table_size >= MAX_DATABASE_TABLE_NUM)
        {
            printf("number of database tables has exceeded the maximum");
            exit(0);
        }
        token = get_token(line, *line_size, value);
        if (token != INT && token != STRING)
        {
            printf("invalid key pair for prefix");
            exit(0);
        }

        strcpy(config->tables[config->table_size]->table_name, value);
        config->tables[config->table_size]->type = token;
        config->table_size++;
        bzero(line, MAX_LINE_SIZE);
        bzero(value, MAX_DATABASE_NAME_SIZE);
    }

    fclose(fp);
    free(line_size);
    return config;
}
