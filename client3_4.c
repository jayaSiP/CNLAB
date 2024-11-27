#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

int main() {
    WSADATA wsData;
    SOCKET sockfd;
    struct sockaddr_in serverAddr;
    struct sockaddr_in serverAddrRecv;
    char *message = "hello there this is a message";
    char received_fromServer[1024];
    int serverAddrLen = sizeof(serverAddrRecv);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket Creation Failed\n");
        WSACleanup();
        return 1;
    } else {
        printf("Socket Created Successfully\n");
    }

    // Set up the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with the server's IP address

    // Send message to server
    int sent = sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (sent == SOCKET_ERROR) {
        printf("Message Sending Failed\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } else {
        printf("Message Sent\n");
    }

    // Receive the message from server
    memset(received_fromServer, '\0', sizeof(received_fromServer));
    int received = recvfrom(sockfd, received_fromServer, sizeof(received_fromServer) - 1, 0, (struct sockaddr *)&serverAddrRecv, &serverAddrLen);
    if (received == SOCKET_ERROR) {
        printf("Message Receiving Failed\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } else {
        received_fromServer[received] = '\0';  // Null-terminate the received data
        printf("Message Received: %s\n", received_fromServer);
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
