#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

void printArr(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void sortArr(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
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
    } else {
        printf("Socket created successfully\n");
    }

    // Server address setup
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Client address setup
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Failed to bind\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } else {
        printf("Binded successfully\n");
    }

    // Receive the array from the client
    int recv_arr[10];
    int n = recvfrom(sockfd, (char*)recv_arr, sizeof(recv_arr), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (n == SOCKET_ERROR) {
        printf("Failed to receive\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } else {
        printf("Array received:\n");
        printArr(recv_arr, 10);
    }

    // Sort the array
    sortArr(recv_arr, 10);

    // Send the sorted array back to the client
    if (sendto(sockfd, (char*)recv_arr, sizeof(recv_arr), 0, (struct sockaddr*)&clientAddr, clientAddrLen) == SOCKET_ERROR) {
        printf("Failed to send\n");
    } else {
        printf("Sorted array sent successfully\n");
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
