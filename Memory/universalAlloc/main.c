#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define BLOCK_SIZE 64
#define NBLOCKS 128
static uint8_t bm_pool[BLOCK_SIZE * NBLOCKS];
static uint64_t bm_used[2];

static void *bm_alloc(void)
{
    for (int w = 0; w < 2; w++)
        for (int i = 0; i < 64; i++)
            if (!((bm_used[w] >> i) & 1))
            {
                bm_used[w] |= (1ULL << i);
                return bm_pool + (w * 64 + i) * BLOCK_SIZE;
            }
    return NULL;
}
static void bm_free(void *p)
{
    int idx = ((uint8_t *)p - bm_pool) / BLOCK_SIZE;
    bm_used[idx / 64] &= ~(1ULL << (idx % 64));
}

typedef struct N
{
    struct N *c[2];
    int h;
    uintptr_t k, k2;
    size_t v;
} N;

static int ht(N *n) { return n ? n->h : 0; }
static void fix(N *n)
{
    int a = ht(n->c[0]), b = ht(n->c[1]);
    n->h = 1 + (a > b ? a : b);
}
static N *rot(N *n, int d)
{
    N *r = n->c[!d];
    n->c[!d] = r->c[d];
    r->c[d] = n;
    fix(n);
    fix(r);
    return r;
}
static N *bal(N *n)
{
    fix(n);
    int b = ht(n->c[0]) - ht(n->c[1]);
    if (b > 1)
    {
        if (ht(n->c[0]->c[0]) < ht(n->c[0]->c[1]))
            n->c[0] = rot(n->c[0], 1);
        return rot(n, 1);
    }
    if (b < -1)
    {
        if (ht(n->c[1]->c[0]) > ht(n->c[1]->c[1]))
            n->c[1] = rot(n->c[1], 0);
        return rot(n, 0);
    }
    return n;
}
static int cmp2(N *n, uintptr_t k, uintptr_t k2)
{
    if (n->k != k)
        return n->k < k ? -1 : 1;
    if (n->k2 != k2)
        return n->k2 < k2 ? -1 : 1;
    return 0;
}
static N *insert(N *n, uintptr_t k, uintptr_t k2, size_t v)
{
    if (!n)
    {
        N *x = bm_alloc();
        x->c[0] = x->c[1] = NULL;
        x->h = 1;
        x->k = k;
        x->k2 = k2;
        x->v = v;
        return x;
    }
    int d = cmp2(n, k, k2);
    if (d > 0)
        n->c[0] = insert(n->c[0], k, k2, v);
    else if (d < 0)
        n->c[1] = insert(n->c[1], k, k2, v);
    else
        n->v = v;
    return bal(n);
}
static N *min_node(N *n) { return n->c[0] ? min_node(n->c[0]) : n; }
static N *erase(N *n, uintptr_t k, uintptr_t k2)
{
    if (!n)
        return NULL;
    int d = cmp2(n, k, k2);
    if (d > 0)
        n->c[0] = erase(n->c[0], k, k2);
    else if (d < 0)
        n->c[1] = erase(n->c[1], k, k2);
    else
    {
        if (!n->c[0] || !n->c[1])
        {
            N *t = n->c[n->c[0] ? 0 : 1];
            bm_free(n);
            return t;
        }
        N *m = min_node(n->c[1]);
        n->k = m->k;
        n->k2 = m->k2;
        n->v = m->v;
        n->c[1] = erase(n->c[1], m->k, m->k2);
    }
    return bal(n);
}
static N *find_addr(N *n, uintptr_t k)
{
    if (!n)
        return NULL;
    if (n->k == k)
        return n;
    return find_addr(n->c[n->k < k ? 1 : 0], k);
}
static N *find_ge(N *n, uintptr_t k)
{
    if (!n)
        return NULL;
    if (n->k == k)
        return n;
    if (k < n->k)
    {
        N *r = find_ge(n->c[0], k);
        return r ? r : n;
    }
    return find_ge(n->c[1], k);
}
static N *pred_addr(N *n, uintptr_t k)
{
    N *res = NULL;
    while (n)
    {
        if (n->k < k)
        {
            res = n;
            n = n->c[1];
        }
        else
            n = n->c[0];
    }
    return res;
}

#define HEAP_SIZE (1 << 20)
static uint8_t heap[HEAP_SIZE];
static N *by_addr = NULL, *by_size = NULL;

void univ_init(void)
{
    uintptr_t base = (uintptr_t)heap;
    by_addr = insert(by_addr, base, 0, HEAP_SIZE);
    by_size = insert(by_size, HEAP_SIZE, base, 0);
}
void *univ_alloc(size_t x)
{
    N *n = find_ge(by_size, (uintptr_t)x);
    if (!n)
        return NULL;
    uintptr_t p = n->k2;
    size_t s = (size_t)n->k;
    by_size = erase(by_size, (uintptr_t)s, p);
    by_addr = erase(by_addr, p, 0);
    if (s - x >= 32)
    {
        by_addr = insert(by_addr, p + x, 0, s - x);
        by_size = insert(by_size, s - x, p + x, 0);
    }
    return (void *)p;
}
void univ_free(void *ptr, size_t x)
{
    uintptr_t p = (uintptr_t)ptr;
    size_t s = x;
    N *nx = find_addr(by_addr, p + x);
    if (nx)
    {
        size_t ns = nx->v;
        by_size = erase(by_size, (uintptr_t)ns, p + x);
        by_addr = erase(by_addr, p + x, 0);
        s += ns;
    }
    N *pv = pred_addr(by_addr, p);
    if (pv && pv->k + pv->v == p)
    {
        uintptr_t pp = pv->k;
        size_t ps = pv->v;
        by_size = erase(by_size, (uintptr_t)ps, pp);
        by_addr = erase(by_addr, pp, 0);
        p = pp;
        s += ps;
    }
    by_addr = insert(by_addr, p, 0, s);
    by_size = insert(by_size, s, p, 0);
}

int main(void)
{
    univ_init();
    void *a = univ_alloc(100), *b = univ_alloc(200), *c = univ_alloc(50);
    printf("a=%p b=%p c=%p\n", a, b, c);
    univ_free(b, 200);
    univ_free(a, 100);
    void *d = univ_alloc(280);
    printf("d=%p (should be near a)\n", d);
    univ_free(d, 280);
    univ_free(c, 50);
    printf("OK\n");
    return 0;
}