#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "config.h"
#include "db.h"

// our defualt values
#define MAX_REQUEST_SIZE 128
#define MAX_RESPONSE_SIZE 32
#define MAX_VALUE_SIZE 128
#define QUEUE_LENGTH 10
#define CLOSE_STATE 2
#define DELIMITER '#'

// typedefing defined structs for convenience
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct in_addr in_addr;

// validates the prefix
Token check_prefix(char *prefix)
{
    if (!strcmp(prefix, INT_KEY))
    {
        return INT;
    }
    if (!strcmp(prefix, STRING_KEY))
    {
        return STRING;
    }
    return INVALID;
}

// validates the value
int check_value(char *value, Token type)
{
    if (type == STRING)
    {
        return 1;
    }
    if (type == INT)
    {
        if (!atoi(value))
        {
            return 0;
        }
    }
    return 1;
}

// gets the next token in a delimitered string
int get_next_token(char *request, int begin, char *token, int *token_size)
{
    int j = 0;
    while (request[begin] && request[begin] != DELIMITER && begin < MAX_DATABASE_TABLE_NUM)
    {
        if (request[begin] == ' ')
        {
            begin++;
            continue;
        }
        token[j++] = request[begin++];
    }
    *token_size = j;
    return begin + 1;
}

// gets the request's information
int get_request_body(char *request, char *table_name, char *value)
{
    char temp[MAX_DATABASE_TABLE_NUM];
    int *temp_size = (int *)malloc(sizeof(int));
    int begin = 0;
    Token token;

    begin = get_next_token(request, begin, temp, temp_size);
    if (!temp_size)
    {
        free(temp_size);
        return 1;
    }

    token = check_prefix(temp);
    if (token == INVALID)
    {
        free(temp_size);
        return 1;
    }
    bzero(temp, MAX_DATABASE_TABLE_NUM);

    begin = get_next_token(request, begin, temp, temp_size);
    if (!temp_size)
    {
        free(temp_size);
        return 1;
    }
    create_table_name(temp, token, table_name);

    begin = get_next_token(request, begin, value, temp_size);
    if (!(*temp_size))
    {
        free(temp_size);
        return 1;
    }
    if (!check_value(value, token))
    {
        free(temp_size);
        return 1;
    }

    free(temp_size);
    return 0;
}

// our main handler for the requests
void handle_request(int sock_hdl)
{
    char request[MAX_REQUEST_SIZE];
    char response[MAX_REQUEST_SIZE];
    char table_name[MAX_DATABASE_NAME_SIZE];
    char value[MAX_VALUE_SIZE];
    bzero(value, MAX_VALUE_SIZE);
    bzero(table_name, MAX_DATABASE_NAME_SIZE);
    bzero(response, MAX_REQUEST_SIZE);
    bzero(request, MAX_REQUEST_SIZE);

    if (recv(sock_hdl, request, MAX_REQUEST_SIZE - 1, 0) == -1)
    {
        printf("error in reading the request");
        shutdown(sock_hdl, CLOSE_STATE);
        exit(0);
    }

    if (get_request_body(request, table_name, value))
    {
        strcpy(response, "not ok");
        if (send(sock_hdl, response, MAX_REQUEST_SIZE, 0) == -1)
        {
            printf("error in sending the response");
            exit(0);
        }
        shutdown(sock_hdl, CLOSE_STATE);
        return;
    }

    sqlite3 *db = get_db_handler();
    if (!check_table_exists(table_name, db))
    {
        strcpy(response, "not ok");
        if (send(sock_hdl, response, MAX_REQUEST_SIZE, 0) == -1)
        {
            printf("error in sending the response");
            free_db_hanlder(db);
            shutdown(sock_hdl, CLOSE_STATE);
            exit(0);
        }
        shutdown(sock_hdl, CLOSE_STATE);
        return;
    }

    insert(table_name, value, db);
    free_db_hanlder(db);

    strcpy(response, "ok");
    if (send(sock_hdl, response, MAX_REQUEST_SIZE, 0) == -1)
    {
        printf("error in sending the response");
        exit(0);
    }
    shutdown(sock_hdl, CLOSE_STATE);
}

// listens on a given port and spawns a new process for each incoming request
void listen_and_serve(int port)
{

    int sock_hdl, incoming_sock_hdl;
    sockaddr_in sock_addr_ser, sock_addr_cli;
    pid_t pid;
    int cli_len;
    int opt = 1;
    cli_len = sizeof(sock_addr_cli);

    bzero((sockaddr_in *)&sock_addr_ser, sizeof(sock_addr_ser));
    bzero((sockaddr_in *)&sock_addr_cli, sizeof(sock_addr_cli));

    if ((sock_hdl = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket creation failed");
        exit(0);
    }
    if (setsockopt(sock_hdl, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        printf("socket attachment failed");
        exit(0);
    }

    sock_addr_ser.sin_addr.s_addr = INADDR_ANY;
    sock_addr_ser.sin_family = AF_INET;
    sock_addr_ser.sin_port = htons(port);
    if (bind(sock_hdl, (sockaddr *)&sock_addr_ser, sizeof(sock_addr_ser)) < 0)
    {
        printf("binding the socket failed");
        exit(0);
    }
    if (listen(sock_hdl, QUEUE_LENGTH))
    {
        printf("listeing failed");
        exit(0);
    }

    while (1)
    {
        incoming_sock_hdl = accept(sock_hdl, (sockaddr *)&sock_addr_cli, ((socklen_t *)&cli_len));
        if ((pid = fork()) == -1)
        {
            printf("can't spawn a new process exiting");
            exit(0);
        }
        if (pid == 0)
        {
            handle_request(incoming_sock_hdl);
            exit(0);
        }
    }
}
