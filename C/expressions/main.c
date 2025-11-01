#include <stdio.h>

#define dim(a) sizeof(a) / sizeof(a[0])
#define offset_of(t, m) (size_t)&(((t *)0)->m)
#define member_size(t, m) sizeof(((t *)0)->m)
#define container_of(p, t, m) ((t *)((char *)(p) - offset_of(t, m)))

int even(int n)
{
    return n % 2 == 0;
}
int neg(int n)
{
    return n < 0;
}
int p2(int p)
{
    return 1 << p;
}
int popcnt(unsigned int n)
{
    int count = 0;
    while (n)
    {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
int is_p2(unsigned int n)
{
    return popcnt(n) == 1; // or if we want pure function : n != 0 && (n & (n - 1)) == 0;
}
int log2N(unsigned int n)
{
    int count = 0;
    while (n >>= 1)
        count++;
    return count;
}
int ceil_p2(int n, int p2)
{
    int base = 1 << p2;
    return ((n + base - 1) / base) * base;
}
int floor_p2(int n, int p2)
{
    int base = 1 << p2;
    return (n / base) * base;
}

int main()
{
    int arr[] = {0, 1, 2, 3, 4, 5};
    printf("%zu\n", dim(arr));
    printf("%d\n", even(2557));
    printf("%d\n", neg(-5));
    printf("%d\n", p2(4));
    printf("%d\n", popcnt(555578));
    printf("%d\n", is_p2(1024));
    printf("%d\n", log2N(512));
    printf("%d\n", ceil_p2(10, 3));
    printf("%d\n", floor_p2(10, 3));

    struct A
    {
        char c; // 3 empty spaces after this
        float a;
        int b;
    };

    printf("%zu\n", offset_of(struct A, a));
    printf("%zu\n", member_size(struct A, a));

    struct A obj;
    obj.a = 3.14;

    float *ptr_to_a = &obj.a;

    struct A *recovered = container_of(ptr_to_a, struct A, a);
    printf("container_of: %p == %p\n", (void *)recovered, (void *)&obj);

    return 0;
}