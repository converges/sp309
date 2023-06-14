#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char message[MAX_BUFFER_SIZE];
    socklen_t server_address_len;

    // Create UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Creating Socket Failed");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);

    printf("[CLIENT] ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // remove \n

    // sendto(): for UDP
    sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&server_address, sizeof(server_address));

    // Receive message from server
    server_address_len = sizeof(server_address);
    memset(message, 0, sizeof(message));
    int recv_len = recvfrom(client_socket, message, sizeof(message), 0, (struct sockaddr*)&server_address, &server_address_len);
    if (recv_len < 0) {
        perror("Receive Failed");
        exit(1);
    }

    printf("[SERVER] %s\n", message);

    close(client_socket);

    return 0;
}
