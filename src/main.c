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
    u_int8_t type;

    if (recv(client_fd, type, 1, 0) <= 0) {
        
    }


    char *buffer = malloc(INITIAL_BUFFER_SIZE);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        close(client_fd);
        return;
    }

    size_t buffer_size = INITIAL_BUFFER_SIZE;
    ssize_t bytes_received;
    size_t total_received = 0;

    while (1) {
        bytes_received = recv(client_fd, buffer + total_received, buffer_size - total_received - 1, 0);
        if (bytes_received <= 0) {
            break;
        }

        total_received += bytes_received;
        buffer[total_received] = '\0';

        if (strchr(buffer, '\n') != NULL) {
            break;
        }

        if (total_received == buffer_size - 1) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
            if (buffer == NULL) {
                perror("Reallocation failed");
                close(client_fd);
                return;
            }
        }
    }

    printf("Received full message: %s", buffer);

    const char *response = "Message received!";
    send(client_fd, response, strlen(response), 0);

    free(buffer);
    close(client_fd);
}

void serve_client(int client_fd, char buffer[]) {

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

