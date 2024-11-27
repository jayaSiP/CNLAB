#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

int main() {
    // Initialize Winsock
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create a socket for sending data
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    else {
        printf("Socket created successfully\n");
    }

    // Server address setup
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Port to connect
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP address of server

    // Send message to server
    const char* message = "Hello to the Server!";
    int sendResult = sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sendResult == SOCKET_ERROR) {
        printf("Failed to send message\n");
    }
    else {
        printf("Message sent\n");
    }

    // Receive message from server
    char recvBuf[1024];
    int serverAddrLen = sizeof(serverAddr);
    int recvResult = recvfrom(sockfd, recvBuf, 1024, 0, (struct sockaddr*)&serverAddr, &serverAddrLen);
    if (recvResult == SOCKET_ERROR) {
        printf("Failed to receive message\n");
    }
    else {
        recvBuf[recvResult] = '\0';  // Null-terminate the received data
        printf("Message received: %s\n", recvBuf);
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
