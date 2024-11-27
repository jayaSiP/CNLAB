#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

int main() {
    // Initialize Winsock
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create Socket
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket Creation Failed\n");
        WSACleanup();
        return 1;
    } else {
        printf("Socket Created Successfully\n");
    }

    // Set up the server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Use INADDR_ANY to bind to any address

    // Bind the Socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Socket Binding Failed\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } else {
        printf("Socket Bound Successfully\n");
    }

    // Receive message from client
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char received_fromClient[1024];
    memset(received_fromClient, '\0', sizeof(received_fromClient));
    int received = recvfrom(sockfd, received_fromClient, sizeof(received_fromClient), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (received == SOCKET_ERROR) {
        printf("Message Receiving Failed\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } else {
        printf("Message Received: %s\n", received_fromClient);
    }

    // Convert the message to uppercase
    char upperCase[1024];
    memset(upperCase, '\0', sizeof(upperCase));
    for (int i = 0; i < received; i++) {
        upperCase[i] = toupper((unsigned char)received_fromClient[i]);
    }

    // Send the uppercase message back to the client
    if (sendto(sockfd, upperCase, strlen(upperCase), 0, (struct sockaddr*)&clientAddr, clientAddrLen) == SOCKET_ERROR) {
        printf("Message Sending Failed\n");
    } else {
        printf("Message Sent: %s\n", upperCase);
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
