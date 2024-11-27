#include <stdio.h>

struct pkt {
    char ch1;
    char ch2[2];
    char ch3;
};

int main() {
    int num;
    struct pkt packet;
    printf("Enter a 4-digit number (0-9999): ");
    scanf("%d", &num);
    packet.ch1 = (num / 1000) % 10;       // Thousands place
    packet.ch2[0] = (num / 100) % 10;     // Hundreds place
    packet.ch2[1] = (num / 10) % 10;      // Tens place
    packet.ch3 = num % 10;                // Ones place
    printf("\n--- Contents of the Structure ---\n");
    printf("ch1: %d\n", packet.ch1);
    printf("ch2[0]: %d\n", packet.ch2[0]);
    printf("ch2[1]: %d\n", packet.ch2[1]);
    printf("ch3: %d\n", packet.ch3);
    int reconstructed_num = packet.ch1 * 1000 + packet.ch2[0] * 100 + packet.ch2[1] * 10 + packet.ch3;
    printf("\nReconstructed Number: %d\n", reconstructed_num);

    return 0;
}
