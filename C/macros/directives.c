#include <stdio.h>
#define LINE printf("%d\n", __LINE__);
#define FILE printf("%s\n", __FILE__);

int main() 
{
    // #line 100 "main.c" 
    LINE

    FILE

    LINE



    #line 10
    LINE

    return 0;
}

#if 0 //1
#error "Som error"
#endif
#pragma warning(disable:1234)