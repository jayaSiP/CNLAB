#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

void rev(char* str, int len) {
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main() {
    // Initialize Winsock
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create a socket for receiving data
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    else {
        printf("Socket created successfully\n");
    }

    // Bind the socket to the server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Port to listen on
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Listen on any IP address

    int isBinded = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (isBinded == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Client address setup
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    // Receive message from client
    char received[1024] = { '\0' };
    int isRecv = recvfrom(sockfd, received, 1024, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (isRecv == SOCKET_ERROR) {
        printf("Cannot receive message\n");
    }
    else {
        received[isRecv] = '\0';  // Null-terminate the received data
        printf("Message received: %s\n", received);
    }

    rev(received, strlen(received));

    // Send reversed message back to client
    int isSend = sendto(sockfd, received, strlen(received), 0, (struct sockaddr*)&clientAddr, clientAddrLen);
    if (isSend == SOCKET_ERROR) {
        printf("Cannot send message\n");
    }
    else {
        printf("Message sent: %s\n", received);
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
