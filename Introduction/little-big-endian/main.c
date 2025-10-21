#include <stdio.h>

int main()
{
    int x = 3;
    for (int i = 0; i < 4; i++)
        for (int b = 7; b >= 0; b--)
            putchar(((unsigned char *)&x)[i] >> b & 1 ? '1' : '0'), b ? 0 : putchar(' '); //on my laptop is little endian

    return 0;
}