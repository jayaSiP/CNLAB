#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

int isPalindrome(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        if (str[i] != str[len - i - 1])
        {
            return 0;
        }
    }
    return 1;
}

int findDifference(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        if (str[i] != str[len - i - 1])
        {
            return abs(str[i] - str[len - i - 1]);
        }
    }
    return 0;
}

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
    {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8888);


        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Server is waiting for data...\n");

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        if (recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        printf("Received string: %s\n", buffer);

        char response[BUFFER_SIZE];
        if (isPalindrome(buffer))
        {
            strcpy(response, "The string is a palindrome.");
        }
        else
        {
            int diff = findDifference(buffer);
            sprintf(response, "The string is not a palindrome. Difference: %d", diff);
        }
        if (sendto(serverSocket, response, strlen(response), 0, (struct sockaddr *)&clientAddr, clientAddrLen) == SOCKET_ERROR)
        {
            printf("sendto() failed. Error Code: %d\n", WSAGetLastError());
        }
    }
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
