#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

struct table
{
    int    xsize, ysize, cellsize;
    char  *buffer;
    size_t total;
};

bool table_init(struct table *t)
{
    t->xsize    = 1024;
    t->ysize    = 1024;
    t->cellsize = 1024;
    t->total    = (size_t)t->xsize * t->ysize * t->cellsize;

    t->buffer = mmap(NULL, t->total,
                     PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE,
                     -1, 0);

    if (t->buffer == MAP_FAILED)
    {
        t->buffer = NULL;
        return false;
    }
    return true;
}

void table_destroy(struct table *t)
{
    if (t->buffer)
        munmap(t->buffer, t->total);
    t->buffer = NULL;
}

char *table_cell(struct table *t, int x, int y)
{
    if (x < 0 || x >= t->xsize || y < 0 || y >= t->ysize)
        return NULL;
    return t->buffer + ((size_t)x + (size_t)y * t->xsize) * t->cellsize;
}

bool table_set_str(struct table *t, int x, int y, const char *str)
{
    char *p = table_cell(t, x, y);
    if (!p)
        return false;
    if (strlen(str) + 1 > (size_t)t->cellsize)
        return false;
    strncpy(p, str, t->cellsize - 1);
    p[t->cellsize - 1] = '\0';
    return true;
}

bool table_get_str(struct table *t, int x, int y, char *out, size_t outsz)
{
    char *p = table_cell(t, x, y);
    if (!p)
        return false;
    strncpy(out, p, outsz - 1);
    out[outsz - 1] = '\0';
    return true;
}

int main(void)
{
    struct table t;
    if (!table_init(&t))
    {
        fprintf(stderr, "table_init failed\n");
        return 1;
    }

    table_set_str(&t, 10,   100,  "Hello, memory");
    table_set_str(&t, 0,    0,    "top-left");
    table_set_str(&t, 1023, 1023, "bottom-right");

    char buf[1024];
    table_get_str(&t, 10,   100,  buf, sizeof(buf)); printf("[10,100]    = %s\n", buf);
    table_get_str(&t, 0,    0,    buf, sizeof(buf)); printf("[0,0]       = %s\n", buf);
    table_get_str(&t, 1023, 1023, buf, sizeof(buf)); printf("[1023,1023] = %s\n", buf);

    table_destroy(&t);
    return 0;
}