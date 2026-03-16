#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>

#define CHUNK 64UL
#define PAGE 4096UL
#define BLOCK (2UL << 20)
#define ARENA (8UL << 30)
#define NBLOCKS (ARENA / BLOCK) // 4096
#define NPAGES (BLOCK / PAGE)   // 512

typedef uint64_t u64;

struct alloc_req
{
    void *ptr;
    size_t size;
};

static void *base;
static int committed[NBLOCKS];

static void init(void)
{
    if (!base)
        base = mmap(0, ARENA, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

struct alloc_req alloc(size_t n)
{
    struct alloc_req r = {0};
    if (!n || n > PAGE)
        return r;
    init();
    size_t need = (n + CHUNK - 1) / CHUNK;
    for (size_t b = 0; b < NBLOCKS; b++)
    {
        u64 *bmap = (u64 *)((char *)base + b * BLOCK);
        if (!committed[b])
        {
            mprotect(bmap, PAGE, PROT_READ | PROT_WRITE);
            memset(bmap, 0, PAGE);
            committed[b] = 1;
        }
        for (size_t p = 1; p < NPAGES; p++)
        {
            if (bmap[p] == ~0ULL)
                continue;
            for (size_t c = 0; c + need <= 64; c++)
            {
                u64 mask = need == 64 ? ~0ULL : (((u64)1 << need) - 1) << c;
                if (!(bmap[p] & mask))
                {
                    void *pg = (char *)bmap + p * PAGE;
                    if (!bmap[p])
                        mprotect(pg, PAGE, PROT_READ | PROT_WRITE);
                    bmap[p] |= mask;
                    r.ptr = (char *)pg + c * CHUNK;
                    r.size = need * CHUNK;
                    return r;
                }
            }
        }
    }
    return r;
}

void dealloc(struct alloc_req *r)
{
    if (!r || !r->ptr)
        return;
    size_t off = (char *)r->ptr - (char *)base;
    size_t b = off / BLOCK;
    size_t p = (off % BLOCK) / PAGE;
    size_t c = (off % PAGE) / CHUNK;
    u64 *bmap = (u64 *)((char *)base + b * BLOCK);
    size_t n = r->size / CHUNK;
    u64 mask = n == 64 ? ~0ULL : (((u64)1 << n) - 1) << c;
    bmap[p] &= ~mask;
    if (!bmap[p])
    {
        madvise((char *)bmap + p * PAGE, PAGE, MADV_DONTNEED);
        mprotect((char *)bmap + p * PAGE, PAGE, PROT_NONE);
        for (size_t i = 1; i < NPAGES; i++)
            if (bmap[i])
                return;
        madvise(bmap, PAGE, MADV_DONTNEED);
        mprotect(bmap, PAGE, PROT_NONE);
        committed[b] = 0;
    }
    r->ptr = 0;
    r->size = 0;
}

int main(void)
{
    struct alloc_req a = alloc(100);
    printf("alloc(100) = %p sz=%zu\n", a.ptr, a.size);
    struct alloc_req b = alloc(64);
    printf("alloc(64)  = %p sz=%zu\n", b.ptr, b.size);
    struct alloc_req big = alloc(PAGE + 1);
    printf("alloc(>PAGE) = %p (want NULL)\n", big.ptr);
    memset(a.ptr, 0xAA, a.size);
    dealloc(&a);
    printf("after free: a.ptr=%p\n", a.ptr);
    struct alloc_req a2 = alloc(100);
    printf("realloc(100) = %p \n", a2.ptr);
    dealloc(&b);
    dealloc(&a2);
    printf("all freed, bitmap decommitted\n");
}