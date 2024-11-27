#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET connectSocket;
    struct sockaddr_in server;
    int result;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET)
    {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    result = connect(connectSocket, (struct sockaddr *)&server, sizeof(server));
    if (result == SOCKET_ERROR)
    {
        printf("Connect failed: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    closesocket(connectSocket);
    WSACleanup();
    return 0;
}
