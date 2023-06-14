#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SERVER_FIFO "./server_fifo"
#define CLIENT_FIFO "./client_fifo"
#define BUFFER_SIZE 256

int main() {
    int server_fd, client_fd;
    char client_fifo[BUFFER_SIZE];

    // Open the server FIFO for writing client messages
    server_fd = open(SERVER_FIFO, O_WRONLY);
    if (server_fd == -1) {
        perror("Failed to open server FIFO");
        exit(EXIT_FAILURE);
    }

    while (1) {
        char message[BUFFER_SIZE];

        // Read a message from the user
        fgets(message, BUFFER_SIZE, stdin);

        // Remove newline character from the message
        message[strcspn(message, "\n")] = '\0';

        // Check if the message is "quit"
        if (strcmp(message, "quit") == 0) {
            // Send "quit" message to the server and exit
            write(server_fd, "quit", sizeof("quit"));
            break;
        }

        // Open the client FIFO for reading
        snprintf(client_fifo, BUFFER_SIZE, CLIENT_FIFO);
        mkfifo(client_fifo, 0666);
        client_fd = open(client_fifo, O_RDONLY);
        if (client_fd == -1) {
            perror("Failed to open client FIFO for reading");
            continue;
        }

        // Write the message to the server FIFO
        write(server_fd, client_fifo, strlen(client_fifo) + 1);
        close(server_fd);

        // Read and print the server's response
        read(client_fd, message, BUFFER_SIZE);
        printf("Server response: %s\n", message);

        close(client_fd);
        unlink(client_fifo);
    }

    return 0;
}
