#include <stdio.h>
void extractBytes(int num, char *byte1, char *byte2, char *byte3, char *byte4)
{
    *byte1 = (num & 0xFF);
    *byte2 = ((num >> 8) & 0xFF);
    *byte3 = ((num >> 16) & 0xFF);
    *byte4 = ((num >> 24) & 0xFF);
}

int main()
{
    int num;
    char byte1, byte2, byte3, byte4;
    printf("Enter an integer: ");
    scanf("%d", &num);
    extractBytes(num, &byte1, &byte2, &byte3, &byte4);
    printf("Byte 1: %d\n", (unsigned char)byte1);
    printf("Byte 2: %d\n", (unsigned char)byte2);
    printf("Byte 3: %d\n", (unsigned char)byte3);
    printf("Byte 4: %d\n", (unsigned char)byte4);

    return 0;
}
