#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

#define MAXDATASIZE 100
#define STDIN 0

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    int fd, numbytes, nval;
    char buf[MAXDATASIZE];
    struct sockaddr_in server;

    if (argc != 3)
    {
        printf("Usage: %s <IP Address> <port>\n", argv[0]);
        exit(1);
    }

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed\n");
        exit(1);
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("socket() error\n");
        WSACleanup();
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
    memset(&(server.sin_zero), 0, 8);

    if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {
        printf("connect() error\n");
        closesocket(fd);
        WSACleanup();
        exit(1);
    }

    fd_set rset, master;
    struct timeval timeout;
    FD_ZERO(&rset);
    FD_ZERO(&master);
    FD_SET(fd, &master);
    FD_SET(STDIN, &master);

    do
    {
        rset = master;
        timeout.tv_sec = 100;
        timeout.tv_usec = 0;

        nval = select(0, &rset, NULL, NULL, &timeout);

        if (FD_ISSET(fd, &rset))
        {
            numbytes = recv(fd, buf, MAXDATASIZE, 0);
            if (numbytes <= 0)
            {
                printf("Connection closed by server.\n");
                break;
            }
            buf[numbytes] = '\0';
            printf("Server Message: %s\n", buf);
        }

        if (FD_ISSET(STDIN, &rset))
        {
            fgets(buf, MAXDATASIZE, stdin);
            send(fd, buf, strlen(buf), 0);
        }

    } while (strncmp(buf, "logout", 6) != 0);

    closesocket(fd);
    WSACleanup();
    return 0;
}
