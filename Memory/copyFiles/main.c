#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

void my_copy(const char *src, const char *dest)
{
    int fd1 = open(src, O_RDONLY);
    if (fd1 < 0) { perror("open src"); return; }

    struct stat st;
    fstat(fd1, &st);
    size_t size = st.st_size;

    int fd2 = open(dest, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd2 < 0) { perror("open dest"); close(fd1); return; }

    ftruncate(fd2, size);

    void *m1 = mmap(NULL, size, PROT_READ,              MAP_PRIVATE, fd1, 0);
    void *m2 = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,  fd2, 0);

    memcpy(m2, m1, size);

    munmap(m1, size);
    munmap(m2, size);

    close(fd1);
    close(fd2);
}

int main(int argc, char *argv[])
{
    if (argc != 3) { fprintf(stderr, "usage: copy <src> <dest>\n"); return 1; }
    my_copy(argv[1], argv[2]);
    return 0;
}