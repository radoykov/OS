#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <elf-file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    struct stat st;
    if (fstat(fd, &st) < 0) { perror("fstat"); close(fd); return 1; }

    char *file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED) { perror("mmap"); close(fd); return 1; }
    close(fd);

    Elf64_Ehdr *eh = (Elf64_Ehdr *)file;
    if (memcmp(eh->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not an ELF file\n");
        munmap(file, st.st_size);
        return 1;
    }

    Elf64_Shdr *sh = (Elf64_Shdr *)(file + eh->e_shoff);
    Elf64_Shdr *sh_strtab_hdr = &sh[eh->e_shstrndx];
    char *strtab = file + sh_strtab_hdr->sh_offset;

    const unsigned char *text = NULL;
    size_t text_size = 0;
    Elf64_Addr text_vaddr = 0;

    for (int i = 0; i < eh->e_shnum; ++i) {
        const char *name = strtab + sh[i].sh_name;
        if (strcmp(name, ".text") == 0) {
            text = (const unsigned char *)(file + sh[i].sh_offset);
            text_size = sh[i].sh_size;
            text_vaddr = sh[i].sh_addr;
            break;
        }
    }

    if (!text) {
        fprintf(stderr, ".text not found\n");
        munmap(file, st.st_size);
        return 1;
    }

    printf(".text: file_offset=0x%lx size=%zu vaddr=0x%lx\n\n",
           (unsigned long)(text - (unsigned char *)file),
           text_size,
           (unsigned long)text_vaddr);

    for (size_t i = 0; i < text_size; ++i) {
        printf("%02x", text[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else printf(" ");
    }
    if (text_size % 16) printf("\n");

    munmap(file, st.st_size);
    return 0;
}
