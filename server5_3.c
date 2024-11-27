#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

int isPalindrome(const char *str) {
    int left = 0, right = strlen(str) - 1;
    while (left < right) {
        if (str[left] != str[right]) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
}

int main() {
    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    struct sockaddr_in server;
    int result;
    char buffer[512];
    int bytesReceived;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;

    result = bind(listenSocket, (struct sockaddr*)&server, sizeof(server));
    if (result == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    result = listen(listenSocket, 1);
    if (result == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port 5000...\n");

    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("Client connected.\n");

    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == SOCKET_ERROR) {
        printf("Receive failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    buffer[bytesReceived] = '\0';  // Null-terminate the string
    printf("Received string: %s\n", buffer);

    if (isPalindrome(buffer)) {
        printf("The string is a palindrome.\n");
    } else {
        printf("The string is not a palindrome.\n");
    }

    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
