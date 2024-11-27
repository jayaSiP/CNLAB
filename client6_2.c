#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVERPORT 4952 

int main() {
    WSADATA wsaData;
    int sockfd;
    struct sockaddr_in their_addr; 
    int numbytes;
    int arrSize;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET; 
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    printf("Enter the size of the array: ");
    scanf("%d", &arrSize);

    int arr[8];
    printf("Enter the elements of the array:\n");
    for (int i = 0; i < arrSize; i++) {
        scanf("%d", &arr[i]);
    }
    if ((numbytes = sendto(sockfd, (char*)arr, arrSize * sizeof(int), 0,
                           (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
        perror("sendto");
        exit(1);
    }

    int primeCount;
    if ((numbytes = recvfrom(sockfd, (char*)&primeCount, sizeof(int), 0,
                             NULL, NULL)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf("The total number of prime numbers in the array: %d\n", primeCount);

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
