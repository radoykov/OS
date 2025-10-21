#include <stdio.h>

int main() 
{
    printf("Hello world!");

    return 0;
}
//steps
//preprocessor ->  gcc -E hello.c -o hello.i
//compile to assembly -> gcc -S hello.i -o hello.s
//compile to object file -> gcc -c hello.s -o hello.o
//linking -> gcc hello.o -o hello.out