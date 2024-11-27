#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define MYPORT 4952
#define MAXBUFLEN 200

int isPrime(int num)
{
    if (num <= 1)
        return 0;
    if (num == 2 || num == 3)
        return 1;
    if (num % 2 == 0 || num % 3 == 0)
        return 0;
    for (int i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return 0;
    }
    return 1;
}

int countPrimes(int *arr, int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (isPrime(arr[i]))
        {
            count++;
        }
    }
    return count;
}

int main()
{
    WSADATA wsaData;
    int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int addr_len, numbytes;
    int buf[MAXBUFLEN / sizeof(int)];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;         
    my_addr.sin_port = htons(MYPORT);     
    my_addr.sin_addr.s_addr = INADDR_ANY; 
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1)
    {
        perror("bind");
        exit(1);
    }

    printf("Server: waiting for data...\n");

    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, (char *)buf, MAXBUFLEN, 0,
                             (struct sockaddr *)&their_addr, &addr_len)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }
    int arrSize = numbytes / sizeof(int);
    printf("Received an array of size %d\n", arrSize);
    int primeCount = countPrimes(buf, arrSize);
    if ((numbytes = sendto(sockfd, (char *)&primeCount, sizeof(int), 0,
                           (struct sockaddr *)&their_addr, addr_len)) == -1)
    {
        perror("sendto");
        exit(1);
    }

    printf("Sent prime count back to client\n");

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
