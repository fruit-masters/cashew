#include "database.h"


int init_database() {

    FILE* file = fopen(PATH, "r");
    if (file == NULL) {
        file = fopen(PATH, "w");
        if (file == NULL) {
            perror("Failed to create database file");
            return -1;
        }

        fprintf(file, "username,ip,port,rsa_public_key,timestamp\n");
        
        fclose(file);
        return 1; // Database created

    } else {
        fclose(file);
    }

    return 0;
}

//Unix format timestamp
char* timestamp(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char* timestamp = malloc(20);
    sprintf(timestamp, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return timestamp;
}


int register_user(char* username, char* rsa_pub_key){
    FILE* file = fopen(PATH, "a");
    if (file == NULL) {
        perror("Failed to open database file");
        return -1;
    }
    int found = 0;
    char line[1024];
    while (fgets(line, 1024, file) != NULL) {
        char* token = strtok(line, ",");
        if (strcmp(token, username) == 0) {
            found = 1;
            break;
        } 
        token = strtok(NULL, ",");
        token = strtok(NULL, ",");
        token = strtok(NULL, ",");

        if (strcmp(token, rsa_pub_key) == 0) {
            found = 2;
            break;
        }
    }

    if (found == 1) {
        perror("Username already exists");
        fclose(file);
        return 1;
    } else if (found == 2) {
        perror("RSA public key already exists");
        fclose(file);
        return 2;
    }

    char* t_str = timestamp();


    fprintf(file, "%s,ip,port,%s,%s\n", username, rsa_pub_key, t_str);
    fclose(file);

    return 0;
}

    

int update_user(char* username, char* ip, char* port){
    FILE* file = fopen(PATH, "a");
    if (file == NULL) {
        perror("Failed to open database file");
        return -1;
    }
    char* rsa_pub_key = NULL;
    int found = 0;
    char line[1024];
    while (fgets(line, 1024, file) != NULL) {
        char* token = strtok(line, ",");
        if (strcmp(token, username) == 0) {
            found = 1;
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            rsa_pub_key = strtok(NULL, ",");

            //delete line
            fseek(file, -strlen(line), SEEK_CUR);
            break;
        }
    }
    //find correct line and update it

    if (!found) {
        perror("User not found, Register first");
        fclose(file);
        return -1;
    }

      char* t_str = timestamp();

    fprintf(file, "%s,%s,%s,%s,%s\n", username, ip, port, rsa_pub_key, t_str);
    fclose(file);

    return 0;
}


QueryRes query_user(char* username){
    FILE* file = fopen(PATH, "r");
    struct QueryRes res;
    if (file == NULL) {
        perror("Failed to open database file");
        res.status = -1;
        return res;
    }
    res.status = 1;
    char line[1024];
    while (fgets(line, 1024, file) != NULL) {
        char* token = strtok(line, ",");
        if (strcmp(token, username) == 0) {
            res.status = 0;
            token = strtok(NULL, ",");
            res.ip = atoi(token);
            token = strtok(NULL, ",");
            res.port = atoi(token);
            token = strtok(NULL, ",");
            res.rsa_pub_key = token;
            token = strtok(NULL, ",");
            res.timestamp = token;
            break;
        }
    }

    fclose(file);
    return res;
}
