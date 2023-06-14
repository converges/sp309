#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SERVER_FIFO "server_fifo"
#define CLIENT_FIFO "client_fifo"
#define BUFFER_SIZE 256

void* client_thread(void* arg);

int main() {
    int send_fd, read_fd;
    char client_fifo[BUFFER_SIZE];
    pthread_t tid;

    // Create a server & client FIFO // 0666: RDWR for everyone
    mkfifo(SERVER_FIFO, 0666); // Message: server -> client
    mkfifo(CLIENT_FIFO, 0666); // Read Message from client

    // FIFO for sending
    send_fd = open(SERVER_FIFO, O_WRONLY);
    if (send_fd == -1) {
        perror("FIFO for sending Failed");
    }

    /* Create a thread for reading client messages */
    pthread_create(&tid, NULL, client_thread, send_fd);

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
            write(send_fd, "quit", sizeof("quit"));
            break;
        }


        // Write the message to the client FIFO
        write(send_fd, message, strlen(message) + 1);
    }
    close(send_fd);

    // Wait for the Client thread
    pthread_join(tid, NULL);

    // Close and remove the server FIFO
    close(send_fd);
    unlink(SERVER_FIFO);

    return 0;
}

void* client_thread(void* arg) {
    int send_fd = *((int*)arg);
    int read_fd;
    char message[BUFFER_SIZE];

    read_fd = open(CLIENT_FIFO, O_RDONLY);
    if (read_fd == -1) {
        perror("FIFO for reading Failed");
        exit(1);
    }

    while (1) {
        // Read a message from the server FIFO
        ssize_t bytes_read = read(read_fd, message, BUFFER_SIZE - 1);

        if (bytes_read > 0) {
            message[bytes_read] = '\0';
        
        // If quit: return
        if (strcmp(message, "quit") == 0)
            break;

        // Print Received Message
        printf("[CLIENT] %s\n", message);

        // Read and print the client message
        write(send_fd, message, BUFFER_SIZE);
        printf("[SERVER] %s\n", message);

        close(read_fd);
    }

    return;
    }
}