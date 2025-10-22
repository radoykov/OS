#include <stdio.h>

#define MIN(...)                                               \
    ({                                                         \
        int arr[] = {__VA_ARGS__};                             \
        int min = arr[0];                                      \
        for (int i = 1; i < sizeof(arr) / sizeof(arr[0]); i++) \
        {                                                      \
            if (arr[i] < min)                                  \
                min = arr[i];                                  \
        }                                                      \
        min;                                                   \
    })

int main()
{
    printf("Min : %d\n", MIN(1, 2));
    printf("Min : %d\n", MIN(8, -3, 0, 99));
    printf("Min : %d\n", MIN(7, 10, 888, 99, -8, -95, 8888));
    return 0;
}