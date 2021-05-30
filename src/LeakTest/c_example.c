#include <stdlib.h>
#include <stdio.h>

void foo(int size)
{
    int* data = malloc(sizeof(int) * size);

    // Uncomment this to stop leak    
    //free(data);
}

void bar(int size)
{
    char *data = malloc(sizeof(char) * size);
    foo(size);

    // Uncomment this to stop leak
    //free(data);
}

void foobar(int size)
{
    bar(size);
}

int main(void)
{
    printf("leakfinder C example app\n");
    printf("This application is expected to leak\n");
    
    foobar(8);
    foobar(16);

    printf("leakfinder C example app all done\n");
    return 0;
}
