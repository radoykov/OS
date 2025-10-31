#include <stdio.h>

int main()
{
    float *(*f)(char *[]);
    int A[3][3] = { //one matrix
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}};

    int B[3][3] = { //zero matrix
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}};
    return 0;
}