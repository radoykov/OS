#include <stdio.h>

unsigned invert(unsigned x, unsigned p, unsigned n)
{
   return x ^ (((1 << n) - 1) << p);
}

int main()
{
    printf("0x%x", invert(0xbaba, 8, 4));
    return 0;
}