#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SERVER_FIFO "./server_fifo"
#define CLIENT_FIFO "./client_fifo"
#define BUFFER_SIZE 256

void* client_thread(void* arg);

int main() {
    int server_fd, client_fd;
    char client_fifo[BUFFER_SIZE];
    pthread_t tid;

    // Create a server FIFO
    // 0666: RDWR for everyone
    mkfifo(SERVER_FIFO, 0666);

    /* Open the server FIFO with RDONLY */
    server_fd = open(SERVER_FIFO, O_RDONLY);
    if (server_fd == -1) {
        perror("Open Server FIFO Failed");
        exit(1);
    }

    /* Create a thread for reading client messages */
    pthread_create(&tid, NULL, client_thread, (void*)&server_fd);

    /* MAIN */
    while (1) {
        char message[BUFFER_SIZE];

        // Get a sentence with fgets
        fgets(message, BUFFER_SIZE, stdin);

        // Remove \n from the message
        message[strcspn(message, "\n")] = '\0';

        /* If message is 'quit' */
        if (strcmp(message, "quit") == 0) {
            // Send "quit" message to the client thread and exit
            write(server_fd, "quit", sizeof("quit"));
            break;
        }

        // Open the client FIFO for writing
        snprintf(client_fifo, BUFFER_SIZE, CLIENT_FIFO);
        client_fd = open(client_fifo, O_WRONLY);
        if (client_fd == -1) {
            perror("Failed to open client FIFO for writing");
            continue;
        }

        // Write the message to the client FIFO
        write(client_fd, message, strlen(message) + 1);
        close(client_fd);
    }

    // Wait for the Client thread
    pthread_join(tid, NULL);

    // Close and remove the server FIFO
    close(server_fd);
    unlink(SERVER_FIFO);

    return 0;
}

void* client_thread(void* arg) {
    int server_fd = *((int*)arg);
    int client_fd;
    char message[BUFFER_SIZE];

    while (1) {
        // Read a message from the server FIFO
        read(server_fd, message, BUFFER_SIZE);

        // Check if the message is "quit"
        if (strcmp(message, "quit") == 0)
            break;

        // Open the client FIFO for reading
        client_fd = open(message, O_RDONLY);
        if (client_fd == -1) {
            perror("Failed to open client FIFO for reading");
            continue;
        }

        // Read and print the client message
        read(client_fd, message, BUFFER_SIZE);
        printf("Received message: %s\n", message);

        close(client_fd);
    }

    return NULL;
}