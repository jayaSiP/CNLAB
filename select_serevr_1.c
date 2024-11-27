#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#define BACKLOG 5
#define MAXDATASIZE 100
int main(int argc, char *argv[])
{WSADATA wsaData;
    int listen_fd, conn_fd, i;
    int client_fd[FD_SETSIZE];
    int max_fd, max_index = 0;
    struct sockaddr_in server, client;
    int sin_size;
    if (argc != 2)
    {printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {printf("WSAStartup() failed\n");
        exit(1);
    }
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {printf("socket() error\n");
        WSACleanup();
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY;
    memset(&(server.sin_zero), 0, 8);
    if (bind(listen_fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {printf("bind() error\n");
        closesocket(listen_fd);
        WSACleanup();
        exit(1);
    }
 if (listen(listen_fd, BACKLOG) == SOCKET_ERROR)
    {printf("listen() error\n");
        closesocket(listen_fd);
        WSACleanup();
        exit(1);
    }
    fd_set read_set, all_set;
    FD_ZERO(&read_set);
    FD_ZERO(&all_set);
    FD_SET(listen_fd, &all_set);
    max_fd = listen_fd;
    for (i = 0; i < FD_SETSIZE; i++)
    {client_fd[i] = -1;
    }
    while (1)
    {   read_set = all_set;
        if (select(0, &read_set, NULL, NULL, NULL) == SOCKET_ERROR)
        {printf("select() error\n");
            closesocket(listen_fd);
            WSACleanup();
            exit(1);
        }
        if (FD_ISSET(listen_fd, &read_set))
        {sin_size = sizeof(struct sockaddr_in);
            if ((conn_fd = accept(listen_fd, (struct sockaddr *)&client, &sin_size)) == INVALID_SOCKET)
            {printf("accept() error\n");
                closesocket(listen_fd);
                WSACleanup();
                exit(1);
            }
            for (i = 0; i < FD_SETSIZE; i++)
            {if (client_fd[i] < 0)
                {
                    client_fd[i] = conn_fd;
                    break;
                }
            }
            FD_SET(conn_fd, &all_set);
            if (conn_fd > max_fd)
            {
                max_fd = conn_fd;
            }
        }
        for (i = 0; i < FD_SETSIZE; i++)
        {if (client_fd[i] < 0)
                continue;
            if (FD_ISSET(client_fd[i], &read_set))
            {char buf[MAXDATASIZE];
                int numbytes = recv(client_fd[i], buf, MAXDATASIZE, 0);
                if (numbytes <= 0)
                {closesocket(client_fd[i]);
                    FD_CLR(client_fd[i], &all_set);
                    client_fd[i] = -1;
                }
                else
                {buf[numbytes] = '\0';
                    if (strncmp(buf, "logout", 6) == 0)
                    {closesocket(client_fd[i]);
                        FD_CLR(client_fd[i], &all_set);
                        client_fd[i] = -1;
                    }
                    else
                {printf("Message from client: %s\n", buf);
                        send(client_fd[i], "Message received", 16, 0);
                    }
                }
            }
        }
    }
    closesocket(listen_fd);
    WSACleanup();
    return 0;
}
