#include <stdlib.h>
#include <stdint.h>

void *malloc_aligned(size_t size, size_t alignment)
{
    // Заделяме допълнително памет за подравняване + място за оригиналния указател
    void *ptr = malloc(size + alignment + sizeof(void *));
    if (!ptr)
        return NULL;

    // Изчисляваме подравнения адрес след мястото за съхранение на оригиналния ptr
    uintptr_t raw_addr = (uintptr_t)ptr + sizeof(void *);
    void *aligned_ptr = (void *)((raw_addr + alignment - 1) & ~(alignment - 1));

    // Записваме оригиналния адрес точно преди подравнения, за да го ползва free_aligned
    ((void **)aligned_ptr)[-1] = ptr;

    return aligned_ptr;
}

void free_aligned(void *aligned_ptr)
{
    if (aligned_ptr)
        free(((void **)aligned_ptr)[-1]);
}

int main()
{
    size_t align = 64;
    void *p = malloc_aligned(100, align);

    if (p)
    {
        printf("Address: %p\n", p);
        printf("Status: %s\n", ((uintptr_t)p % align == 0) ? "Aligned" : "FAILED");

        free_aligned(p);
    }

    return 0;
}