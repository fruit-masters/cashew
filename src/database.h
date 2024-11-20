#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define PATH "db/data.csv"

typedef struct QueryRes {
    int8_t status;
    uint32_t ip;
    uint16_t port;
    char* rsa_pub_key;
    char* timestamp;
} QueryRes;


int init_database();
char* timestamp();
int register_user(char* username, char* rsa_pub_key);
int update_user(char* username, char* ip, char* port);
QueryRes query_user(char* username);