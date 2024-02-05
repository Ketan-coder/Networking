#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address

    // Send text string to server
    char message[] = "Hello, UDP Server!";
    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error sending data");
        exit(EXIT_FAILURE);
    }

    // Receive reversed string from server
    char buffer[MAX_BUFFER_SIZE];
    ssize_t recv_size = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
    if (recv_size == -1) {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    buffer[recv_size] = '\0';  // Null-terminate the received data
    printf("Received from server: %s\n", buffer);

    // Close socket
    close(client_socket);

    return 0;
}
