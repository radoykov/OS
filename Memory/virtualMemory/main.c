#define _GNU_SOURCE
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    size_t user_size;
    size_t mmap_size;
} Header;

#define PAGE 4096UL
#define ALIGN(n) (((n) + PAGE - 1) & ~(PAGE - 1)) //Round it up to a page boundary

void *alloc(size_t size)
{
    size_t total = ALIGN(sizeof(Header) + size) * 4;
    void *map = mmap(NULL, total, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED)
        return NULL;
    Header *h = map;
    h->user_size = size;
    h->mmap_size = total;
    return h + 1;
}

void dealloc(void *ptr)
{
    if (!ptr)
        return;
    Header *h = (Header *)ptr - 1;
    munmap(h, h->mmap_size);
}

void *realloc_nomove(void *ptr, size_t new_size)
{
    if (!ptr)
        return alloc(new_size);
    if (!new_size)
    {
        dealloc(ptr);
        return NULL;
    }
    Header *h = (Header *)ptr - 1;
    size_t new_total = ALIGN(sizeof(Header) + new_size);
    if (new_total <= h->mmap_size)
    {
        h->user_size = new_size;
        return ptr;
    }
    void *m = mremap(h, h->mmap_size, new_total, 0);
    if (m == MAP_FAILED)
        return NULL;
    h = m;
    h->user_size = new_size;
    h->mmap_size = new_total;
    return h + 1;
}

int main(void)
{
    char *p = alloc(64);
    memcpy(p, "hello", 6);
    printf("alloc:   %p  data=%s\n", (void *)p, p);
    char *p2 = realloc_nomove(p, 128);
    printf("realloc: %p  data=%s  same=%s\n", (void *)p2, p2, p2 == p ? "yes" : "no");
    dealloc(p2);
    printf("dealloc: OK\n");
    
    return 0;
}