#include <iostream>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }

    int client_socket;
    sockaddr_in server_addr;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Error creating socket\n";
        WSACleanup();
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address

    // Connect to server
    if (connect(client_socket, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1)
    {
        std::cerr << "Error connecting to server\n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Send numbers to server
    int numbers[] = {5, 10, 2, 4, 7, 3};
    send(client_socket, reinterpret_cast<char *>(numbers), sizeof(numbers), 0);

    // Close socket
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
