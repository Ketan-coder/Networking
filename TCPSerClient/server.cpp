#include <iostream>
#include <cstring>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

void sort_numbers(int numbers[], int size)
{
    int i, j, temp;
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (numbers[j] < numbers[j + 1])
            {
                temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
}

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }

    int server_socket, client_socket;
    sockaddr_in server_addr, client_addr;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Error creating socket\n";
        WSACleanup();
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1)
    {
        std::cerr << "Error binding\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for connections
    if (listen(server_socket, 10) == -1)
    {
        std::cerr << "Error listening\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept connection from client
    int addr_size = sizeof(client_addr);
    if ((client_socket = accept(server_socket, reinterpret_cast<sockaddr *>(&client_addr), &addr_size)) == -1)
    {
        std::cerr << "Error accepting connection\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connection accepted from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

    // Receive numbers from client
    int numbers[6];
    recv(client_socket, reinterpret_cast<char *>(numbers), sizeof(numbers), 0);

    // Sort the numbers in descending order
    sort_numbers(numbers, sizeof(numbers) / sizeof(numbers[0]));

    // Display sorted numbers
    std::cout << "Sorted numbers: ";
    for (int i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    // Close sockets
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
