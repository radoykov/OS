#include <stdio.h>

#define FLOOR(n, k) (n / k) * k
#define CEIL(n, k) FLOOR(n, k) + k

int main()
{
    int n = 7, k = 5;
    printf("Floor : %d\n", FLOOR(n, k));
    printf("Ceil : %d\n", CEIL((5 + 2), k));    

    return 0;
}