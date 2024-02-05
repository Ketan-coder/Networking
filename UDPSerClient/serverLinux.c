#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        char buffer[MAX_BUFFER_SIZE];
        ssize_t recv_size;

        // Receive data from client
        recv_size = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_size);
        if (recv_size == -1) {
            perror("Error receiving data");
            exit(EXIT_FAILURE);
        }

        buffer[recv_size] = '\0';  // Null-terminate the received data

        // Reverse the string
        int length = strlen(buffer);
        for (int i = 0; i < length / 2; i++) {
            char temp = buffer[i];
            buffer[i] = buffer[length - i - 1];
            buffer[length - i - 1] = temp;
        }

        // Send reversed string back to client
        if (sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) == -1) {
            perror("Error sending data");
            exit(EXIT_FAILURE);
        }
    }

    // The server does not reach here in this example, but you can add cleanup code if needed.
    close(server_socket);

    return 0;
}
