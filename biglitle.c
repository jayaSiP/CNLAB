#include <stdio.h>
int main()
{
    unsigned int x;
    printf("enter the value of x:");
    scanf("%d",&x);
    char *c = (char *)&x;
    printf("*c is: 0x%x \n", *c);
    if (*c == 0x10)
        printf("the underlying machine is in little endian");
    else
        printf("the machine is in big endian");
    return 0;
}
