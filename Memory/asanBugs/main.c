#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void use_after_free(void) {
    int *p = malloc(sizeof(int));
    free(p);
    *p = 42;       
}

static void double_free(void) {
    int *p = malloc(sizeof(int));
    free(p);
    free(p); 
}

static void invalid_free(void) {
    int x = 5;
    free(&x);  
}

static void leak(void) {
    malloc(64);
}

static void buf_overflow(void) {
    char *s = malloc(11);      
    strcpy(s, "Hello world"); 
    printf("%s\n", s);
    free(s);
}

static void off_by_one(void) {
    int *a = malloc(4 * sizeof(int));
    a[4] = 99;   
    free(a);
}

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 0;
    switch (n) {
        case 1: use_after_free(); break;
        case 2: double_free();    break;
        case 3: invalid_free();   break;
        case 4: leak();           break;
        case 5: buf_overflow();   break;
        case 6: off_by_one();     break;
        default:
            puts("Usage: ./asan_bugs <1-6>");
            puts("  1=use-after-free  2=double-free  3=invalid-free");
            puts("  4=leak            5=buf-overflow 6=off-by-one");
    }
}