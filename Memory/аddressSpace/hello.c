#include <stdio.h>

void main (){
    printf("Hello world!");
}
// $ gcc hello.c -g
// $ gdb ./a.out
// (gdb) start
// (gdb) info proc
// shell pmap -p 81530     <- current process(81530)
// (gdb) shell pmap -p -X 81530
// (gdb) shell pmap -p -XX 81530