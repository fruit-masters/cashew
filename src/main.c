#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define PORT 15201
#define INITIAL_BUFFER_SIZE 1024

int create_server_socket() {
    int server_fd;
    struct sockaddr_in server_addr;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void handle_client(int client_fd) {
    uint8_t type;

    if (recv(client_fd, type, sizeof(type), 0) <= 0) {
        perror("Nothing received");
        close(client_fd);
        return;
    }

    int8_t status;

    switch (type) {
        case 1:
            status = method_register(client_fd);
            break;
        case 2:
            status = method_update(client_fd);
            break;
        case 3:
            // query
            break;
        default:
            break;
    }

    const char *response = "OK";
    send(client_fd, response, strlen(response), 0);
}

int8_t method_register(int client_fd) {
    char* username;
    if ((username = parse_string_with_size(client_fd)) == NULL) return -1;
    char* rsa_public_key;
    if ((rsa_public_key = parse_string_with_size(client_fd)) == NULL) return -1;

    // database logic

    return 0;
}

int8_t method_update(int client_fd) {
    char* username;
    if ((username = parse_string_with_size(client_fd)) == NULL) return -1;
    
    char signed_ip_port[7];
    if (recv(client_fd, signed_ip_port, 6, 0) <= 0) {
        perror("Nothing received");
        close(client_fd);
        return -1;
    }

    signed_ip_port[6] = '\0';

    // rsa decrypt with public key

    return 0;
}



char* parse_string_with_size(int client_fd) {
    uint32_t len;
    char* string;

    if (recv(client_fd, &len, sizeof(len), 0) <= 0) {
        perror("Nothing received");
        close(client_fd);
        return NULL;
    }

    len = ntohl(len);

    string = malloc(len + 1);

    if (recv(client_fd, string, sizeof(string), 0) <= 0) {
        perror("Nothing received");
        close(client_fd);
        return NULL;
    }

    string[len] = '\0';

    return string;
}

int main(int argc, char* argv[]) {
    printf("cashew\n\n");

    int server_fd, client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_fd = create_server_socket();

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Accepted a new connection\n");

        handle_client(client_fd);
    }

    close(server_fd);

    return 0;
}

