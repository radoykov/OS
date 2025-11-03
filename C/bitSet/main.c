#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 8192
#define BITSET_SIZE (MAX_N / 8) // 1024

typedef struct
{
    unsigned char bits[BITSET_SIZE]; // char is 8 bits thus 1024 bytes overall
} IntSet;

// inner function
static int is_valid(int n)
{
    return n >= 0 && n < MAX_N;
}

// add element
void insert(IntSet *set, int n)
{
    if (is_valid(n))
        set->bits[n / 8] |= (1 << (n % 8)); // or set->bits[n / 8] = set->bits[n / 8] | (1 << (n % 8)); n/8 calculates the group n % 8 is the index
}

// remove element
void remove_elem(IntSet *set, int n)
{
    if (is_valid(n))
        set->bits[n / 8] &= ~(1 << (n % 8));
}

// if num contains
int contains(const IntSet *set, int n)
{
    if (is_valid(n))
        return (set->bits[n / 8] >> (n % 8)) & 1;
    return 0;
}

// union
IntSet union_sets(const IntSet *a, const IntSet *b)
{
    IntSet result;
    for (int i = 0; i < BITSET_SIZE; i++)
        result.bits[i] = a->bits[i] | b->bits[i]; // |
    return result;
}

// intersection
IntSet intersect_sets(const IntSet *a, const IntSet *b)
{
    IntSet result;
    for (int i = 0; i < BITSET_SIZE; i++)
        result.bits[i] = a->bits[i] & b->bits[i]; // &
    return result;
}

// difference a to b if we want in both we have to use ^ xor
IntSet diff_sets(const IntSet *a, const IntSet *b)
{
    IntSet result;
    for (int i = 0; i < BITSET_SIZE; i++)
        result.bits[i] = a->bits[i] & ~(b->bits[i]);
    return result;
}

// print the set
void print_set(const IntSet *set)
{
    printf("{ ");
    for (int i = 0; i < MAX_N; i++)
    {
        if (contains(set, i))
            printf("%d ", i);
    }
    printf("}\n");
}

int main()
{
    IntSet set = {0};
    insert(&set, 8);
    insert(&set, 125);
    insert(&set, 1238);
    remove_elem(&set, 8);
    printf("if contains : %d\n", contains(&set, 125));
    print_set(&set);

    IntSet set2 = {0};
    insert(&set2, 1);
    insert(&set2, 1001);
    print_set(&set2);

    IntSet u, i, d;
    u = union_sets(&set, &set2);
    i = intersect_sets(&set, &set2);
    d = diff_sets(&set, &set2);
    print_set(&u);
    print_set(&i);
    print_set(&d);

    return 0;
}