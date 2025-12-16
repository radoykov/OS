#include <stdio.h>
typedef unsigned int uint;

// given
uint nor(uint x, uint y) { return !(x | y); }

uint not(uint x) { return nor(x, x); }
uint or(uint x, uint y) { return !nor(x, y); }
uint and(uint x, uint y) { return nor(!x, !y); }
uint nand(uint x, uint y) { return !nor(!x, !y); }
uint imply(uint x, uint y) { return nor(x, y) | y; }
uint equiv(uint x, uint y) { return (nor(x, y) | y) & (nor(y, x) | x); }
uint xor(uint x, uint y) { return !nor(x, y) & (!nor(!x, !y)); }

int main()
{
    printf("Nor :   %d %d %d %d\n", nor(0, 0), nor(0, 1), nor(1, 0), nor(1, 1));

    printf("Not :   %d %d\n", not(0), not(1));
    printf("Or :    %d %d %d %d\n", or(0, 0), or(0, 1), or(1, 0), or(1, 1));
    printf("And :   %d %d %d %d\n", and(0, 0), and(0, 1), and(1, 0), and(1, 1));
    printf("Nand :  %d %d %d %d\n", nand(0, 0), nand(0, 1), nand(1, 0), nand(1, 1));
    printf("Imply : %d %d %d %d\n", imply(0, 0), imply(0, 1), imply(1, 0), imply(1, 1));
    printf("Equiv : %d %d %d %d\n", equiv(0, 0), equiv(0, 1), equiv(1, 0), equiv(1, 1));
    printf("Xor :   %d %d %d %d\n", xor(0, 0), xor(0, 1), xor(1, 0), xor(1, 1));

    return 0;
}