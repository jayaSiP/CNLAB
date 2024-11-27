#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in server;
    char message[] = "Welcome to KIIT";
    char vowels_count[10];
    int result;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connect failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("Connected to server\n");

    send(sockfd, message, strlen(message), 0);

    result = recv(sockfd, vowels_count, sizeof(vowels_count) - 1, 0);
    if (result == SOCKET_ERROR) {
        printf("Receive failed: %d\n", WSAGetLastError());
    } else {
        vowels_count[result] = '\0';
        printf("Number of vowels received from server: %s\n", vowels_count);
    }

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
