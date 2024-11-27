#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

int main() {
    WSADATA wsData;
    SOCKET server_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[1024] = {'\0'};
    int client_address_len = sizeof(client_address);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    } else {
        printf("Socket created successfully\n");
    }

    // Set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;  // Use INADDR_ANY for any incoming address

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Bind successful. Waiting for messages...\n");

    // Receive message from client
    int recv_len = recvfrom(server_socket, buffer, sizeof(buffer), 0,
                            (struct sockaddr *)&client_address, &client_address_len);
    if (recv_len == SOCKET_ERROR) {
        printf("Failed to receive message\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Received message from client: %s\n", buffer);

    // Cleanup
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
