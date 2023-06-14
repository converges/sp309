#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8080

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    char message[MAX_BUFFER_SIZE];
    socklen_t client_address_len;

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket<0) {
        perror("Creating Socket Failed");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);

    // Assign IP address and port number to socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind Failed");
        exit(1);
    }

    printf("Server Has Been Running.\n");

    while (1) {
        client_address_len = sizeof(client_address);
        memset(message, 0, sizeof(message));

        // Receive message from client
        int recv_len = recvfrom(server_socket, message, sizeof(message), 0, (struct sockaddr*)&client_address, &client_address_len);
        if (recv_len < 0) {
            perror("Receive Failed");
            exit(1);
        }

        printf("[CLIENT] %s\n", message);

        // sendto() for UDP
        sendto(server_socket, message, recv_len, 0, (struct sockaddr*)&client_address, client_address_len);
    }

    close(server_socket);

    return 0;
}