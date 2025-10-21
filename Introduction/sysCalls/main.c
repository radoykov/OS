#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    if (argc < 4)
        return 1;
    const char *filename = argv[1], *data = argv[3];
    off_t offset = atoi(argv[2]);

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return 1;
 if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
        return 1;
    }
    write(fd, data, strlen(data));
    close(fd);
    return 0;
}
// to run it -> gcc main.c -o main.out
// strace ./main.out file 5 "HI there"
