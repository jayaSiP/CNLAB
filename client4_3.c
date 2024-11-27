#include <stdio.h>
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
    } else {
        printf("Socket created successfully\n");
    }

    // Server address setup
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Port to connect
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP address of server

    // Client address setup
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    // Array to send
    int array[10] = {7, 9, 9, 9, 1, 8, 5, 2, 2, 7};

    // Send array to server
    int isSent = sendto(sockfd, (char*)array, sizeof(array), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (isSent == SOCKET_ERROR) {
        printf("Failed to send\n");
    } else {
        printf("Array sent successfully\n");
    }

    // Receive greatest number from server
    int greatest;
    int isRecv = recvfrom(sockfd, (char*)&greatest, sizeof(greatest), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (isRecv == SOCKET_ERROR) {
        printf("Failed to receive\n");
    } else {
        printf("Greatest received: %d\n", greatest);
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
