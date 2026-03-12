
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PTE_PRESENT (1ULL << 0)
#define PTE_WRITE (1ULL << 1)
#define PTE_PS (1ULL << 7) 

#define PTE_ADDR_MASK 0x000FFFFFFFFFF000ULL

#define PML4_IDX(va) (((va) >> 39) & 0x1FFUL)
#define PDPT_IDX(va) (((va) >> 30) & 0x1FFUL)
#define PD_IDX(va) (((va) >> 21) & 0x1FFUL)
#define PT_IDX(va) (((va) >> 12) & 0x1FFUL)

#define PAGE_4K (1ULL << 12)
#define PAGE_2M (1ULL << 21)
#define PAGE_1G (1ULL << 30)

#define ALIGNED_4K(a) (((a) & (PAGE_4K - 1)) == 0)
#define ALIGNED_2M(a) (((a) & (PAGE_2M - 1)) == 0)
#define ALIGNED_1G(a) (((a) & (PAGE_1G - 1)) == 0)

#define ENTRIES 512

typedef uint64_t pte_t;

typedef struct pt_node
{
    pte_t e[ENTRIES];
} pt_node_t;

struct pagetable
{
    pt_node_t *pml4;
};

static pt_node_t *alloc_node(void)
{
    pt_node_t *n = aligned_alloc(PAGE_4K, sizeof(pt_node_t));
    if (n)
        memset(n, 0, sizeof(pt_node_t));
    return n;
}

static inline pte_t ptr_to_pte(pt_node_t *n, pte_t flags)
{
    return ((uintptr_t)n & PTE_ADDR_MASK) | flags;
}

static inline pt_node_t *pte_to_ptr(pte_t entry)
{
    return (pt_node_t *)(uintptr_t)(entry & PTE_ADDR_MASK);
}

static pt_node_t *get_or_alloc_child(pte_t *entry)
{
    if (!(*entry & PTE_PRESENT))
    {
        pt_node_t *child = alloc_node();
        if (!child)
            return NULL;
        *entry = ptr_to_pte(child, PTE_PRESENT | PTE_WRITE);
    }
    return pte_to_ptr(*entry);
}

struct pagetable *pagetable_create(void)
{
    struct pagetable *pt = malloc(sizeof(*pt));
    if (!pt)
        return NULL;
    pt->pml4 = alloc_node();
    if (!pt->pml4)
    {
        free(pt);
        return NULL;
    }
    return pt;
}

bool map(struct pagetable *pt, uint64_t virt, uint64_t phys)
{
    if (!pt || !pt->pml4)
        return false;

    /* PML4 → PDPT */
    pt_node_t *pdpt = get_or_alloc_child(&pt->pml4->e[PML4_IDX(virt)]);
    if (!pdpt)
        return false;
    pte_t *pdpte = &pdpt->e[PDPT_IDX(virt)];

    /* 1 GB huge page */
    if (ALIGNED_1G(virt) && ALIGNED_1G(phys))
    {
        if (*pdpte & PTE_PRESENT)
            return false;
        *pdpte = (phys & PTE_ADDR_MASK) | PTE_PRESENT | PTE_WRITE | PTE_PS;
        return true;
    }
    if (*pdpte & PTE_PS)
        return false; 

    /* PDPT → PD */
    pt_node_t *pd = get_or_alloc_child(pdpte);
    if (!pd)
        return false;
    pte_t *pde = &pd->e[PD_IDX(virt)];

    /* 2 MB huge page */
    if (ALIGNED_2M(virt) && ALIGNED_2M(phys))
    {
        if (*pde & PTE_PRESENT)
            return false;
        *pde = (phys & PTE_ADDR_MASK) | PTE_PRESENT | PTE_WRITE | PTE_PS;
        return true;
    }
    if (*pde & PTE_PS)
        return false; \

    /* PD → PT (4 KB) */
    if (!ALIGNED_4K(virt) || !ALIGNED_4K(phys))
        return false;
    pt_node_t *pgtbl = get_or_alloc_child(pde);
    if (!pgtbl)
        return false;
    pte_t *pte = &pgtbl->e[PT_IDX(virt)];
    if (*pte & PTE_PRESENT)
        return false;
    *pte = (phys & PTE_ADDR_MASK) | PTE_PRESENT | PTE_WRITE;
    return true;
}

bool unmap(struct pagetable *pt, uint64_t virt)
{
    if (!pt || !pt->pml4)
        return false;

    pte_t pml4e = pt->pml4->e[PML4_IDX(virt)];
    if (!(pml4e & PTE_PRESENT))
        return false;

    pt_node_t *pdpt = pte_to_ptr(pml4e);
    pte_t *pdpte = &pdpt->e[PDPT_IDX(virt)];
    if (!(*pdpte & PTE_PRESENT))
        return false;

    if (*pdpte & PTE_PS)
    {
        *pdpte = 0;
        return true;
    } /* 1 GB */

    pt_node_t *pd = pte_to_ptr(*pdpte);
    pte_t *pde = &pd->e[PD_IDX(virt)];
    if (!(*pde & PTE_PRESENT))
        return false;

    if (*pde & PTE_PS)
    {
        *pde = 0;
        return true;
    } /* 2 MB */

    pt_node_t *pgtbl = pte_to_ptr(*pde);
    pte_t *pte = &pgtbl->e[PT_IDX(virt)];
    if (!(*pte & PTE_PRESENT))
        return false;
    *pte = 0; /* 4 KB */
    return true;
}

bool virt2phys(const struct pagetable *pt, uint64_t virt, uint64_t *phys)
{
    if (!pt || !pt->pml4 || !phys)
        return false;

    pte_t pml4e = pt->pml4->e[PML4_IDX(virt)];
    if (!(pml4e & PTE_PRESENT))
        return false;

    pte_t pdpte = pte_to_ptr(pml4e)->e[PDPT_IDX(virt)];
    if (!(pdpte & PTE_PRESENT))
        return false;
    if (pdpte & PTE_PS)
    { /* 1 GB */
        *phys = (pdpte & PTE_ADDR_MASK) | (virt & (PAGE_1G - 1));
        return true;
    }

    pte_t pde = pte_to_ptr(pdpte)->e[PD_IDX(virt)];
    if (!(pde & PTE_PRESENT))
        return false;
    if (pde & PTE_PS)
    { /* 2 MB */
        *phys = (pde & PTE_ADDR_MASK) | (virt & (PAGE_2M - 1));
        return true;
    }

    pte_t pte = pte_to_ptr(pde)->e[PT_IDX(virt)];
    if (!(pte & PTE_PRESENT))
        return false; /* 4 KB */
    *phys = (pte & PTE_ADDR_MASK) | (virt & (PAGE_4K - 1));
    return true;
}

static void free_pd(pt_node_t *pd)
{
    for (int i = 0; i < ENTRIES; i++)
    {
        pte_t e = pd->e[i];
        if ((e & PTE_PRESENT) && !(e & PTE_PS))
            free(pte_to_ptr(e));
    }
    free(pd);
}

static void free_pdpt(pt_node_t *pdpt)
{
    for (int i = 0; i < ENTRIES; i++)
    {
        pte_t e = pdpt->e[i];
        if ((e & PTE_PRESENT) && !(e & PTE_PS))
            free_pd(pte_to_ptr(e));
    }
    free(pdpt);
}

void pagetable_destroy(struct pagetable *pt)
{
    if (!pt)
        return;
    for (int i = 0; i < ENTRIES; i++)
    {
        pte_t e = pt->pml4->e[i];
        if (e & PTE_PRESENT)
            free_pdpt(pte_to_ptr(e));
    }
    free(pt->pml4);
    free(pt);
}

static void check(const char *label, bool ok)
{
    printf("  %-47s %s\n", label, ok ? "OK" : "FAIL");
}

int main(void)
{
    struct pagetable *pt = pagetable_create();
    uint64_t out;

    puts("=== 4 KB pages ===");
    uint64_t v4 = 0x0000000000001000ULL, p4 = 0x0000000012345000ULL;
    check("map 4KB", map(pt, v4, p4));
    check("map same → false", !map(pt, v4, p4));
    check("virt2phys base", virt2phys(pt, v4, &out) && out == p4);
    check("virt2phys +0xABC", virt2phys(pt, v4 + 0xABC, &out) && out == p4 + 0xABC);
    check("unmap", unmap(pt, v4));
    check("unmap again → false", !unmap(pt, v4));
    check("v2p after unmap", !virt2phys(pt, v4, &out));

    puts("=== 2 MB pages ===");
    uint64_t v2 = 0x0000000000200000ULL, p2 = 0x0000000080000000ULL;
    check("map 2MB", map(pt, v2, p2));
    check("virt2phys base", virt2phys(pt, v2, &out) && out == p2);
    check("virt2phys +0x1FF000", virt2phys(pt, v2 + 0x1FF000, &out) && out == p2 + 0x1FF000);
    check("unmap 2MB", unmap(pt, v2));

    puts("=== 1 GB pages ===");
    uint64_t v1 = 0x0000000040000000ULL, p1 = 0x0000000040000000ULL;
    check("map 1GB", map(pt, v1, p1));
    check("virt2phys base", virt2phys(pt, v1, &out) && out == p1);
    check("virt2phys +0x3FF000", virt2phys(pt, v1 + 0x3FF000, &out) && out == p1 + 0x3FF000);
    check("unmap 1GB", unmap(pt, v1));

    puts("=== misaligned ===");
    check("map misaligned virt", !map(pt, 0x1234, 0x5000));
    check("map misaligned phys", !map(pt, 0x5000, 0x1234));

    pagetable_destroy(pt);
    puts("\nDone.");
    return 0;
}