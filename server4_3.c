#include <stdio.h>
#include <winsock2.h>
#include <limits.h>

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

void printArr(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int getGreatest(int arr[], int n) {
    int mx = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > mx) mx = arr[i];
    }
    return mx;
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
    } else {
        printf("Socket created successfully\n");
    }

    // Server address setup
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Port to listen on
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Listen on any IP address

    // Bind the socket to the server address
    int isBinded = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (isBinded == SOCKET_ERROR) {
        printf("Failed to bind\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } else {
        printf("Binded successfully\n");
    }

    // Client address setup
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    // Receive array from client
    int recv_arr[10];
    int n = recvfrom(sockfd, (char*)recv_arr, sizeof(recv_arr), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (n == SOCKET_ERROR) {
        printf("Failed to receive\n");
    } else {
        printf("Array received:\n");
        printArr(recv_arr, 10);
    }

    // Find the greatest number in the array
    int greatest = getGreatest(recv_arr, 10);

    // Send the greatest number back to the client
    int isSent = sendto(sockfd, (char*)&greatest, sizeof(greatest), 0, (struct sockaddr*)&clientAddr, clientAddrLen);
    if (isSent == SOCKET_ERROR) {
        printf("Failed to send\n");
    } else {
        printf("Greatest sent successfully\n");
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
