#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int clientSize = sizeof(client);
    char buffer[100];
    int result;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    printf("Bind successful\n");

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Waiting for incoming connections...\n");

    clientSocket = accept(serverSocket, (struct sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    result = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (result == SOCKET_ERROR) {
        printf("Receive failed: %d\n", WSAGetLastError());
    } else {
        buffer[result] = '\0';
        printf("String received from client: %s\n", buffer);

        int vowels = 0;
        for (int i = 0; i < strlen(buffer); i++) {
            char ch = tolower(buffer[i]);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowels++;
            }
        }

        char vowels_count[10];
        sprintf(vowels_count, "%d", vowels);
        send(clientSocket, vowels_count, strlen(vowels_count), 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
