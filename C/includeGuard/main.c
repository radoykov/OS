#include <stdio.h>
#include "b.h"
#include "c.h"

int main() 
{
    printf("%d\n", CONUST_MACRO);
    return 0;
}
//to compile only preprocessor -> gcc -E main.c -o main.i
// without include gard ->
// # 2 "main.c" 2
// # 1 "b.h" 1
// # 1 "a.h" 1
// # 2 "b.h" 2
// # 3 "main.c" 2
// # 1 "c.h" 1
// # 1 "a.h" 1
// # 2 "c.h" 2
// # 4 "main.c" 2

// with ->
// # 2 "main.c" 2
// # 1 "b.h" 1
// # 1 "a.h" 1
// # 2 "b.h" 2
// # 3 "main.c" 2
// # 1 "c.h" 1
// # 4 "main.c" 2

//so this prevents additional files a.h which are redudant

