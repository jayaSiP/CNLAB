#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    int serverAddrLen = sizeof(serverAddr);
    char buffer[BUFFER_SIZE];
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());

        if ((clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
        {
            printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8888);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        while (1)
        {
            printf("Enter a string: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, serverAddrLen) == SOCKET_ERROR)
            {
                printf("sendto() failed. Error Code: %d\n", WSAGetLastError());
                continue;
            }
            memset(buffer, 0, BUFFER_SIZE);
            if (recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&serverAddr, &serverAddrLen) == SOCKET_ERROR)
            {
                printf("recvfrom() failed. Error Code: %d\n", WSAGetLastError());
                continue;
            }

            printf("Server response: %s\n", buffer);
        }
        closesocket(clientSocket);
        WSACleanup();

        return 0;
    }
