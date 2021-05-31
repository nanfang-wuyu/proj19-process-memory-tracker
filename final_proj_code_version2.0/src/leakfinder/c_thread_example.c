#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

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

void* thread_run(void* ptr)
{
    char* message = (char*)ptr;
    printf("Caller %s with id %ld\n", message, pthread_self());

    foobar(32);    
}

int main(void)
{
    printf("leakfinder C thread example app\n");
    printf("This application is expected to leak on multiple threads\n");
    
    pthread_t thread1, thread2;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";
   
    pthread_create(&thread1, NULL, thread_run, (void*)message1);
    pthread_create(&thread2, NULL, thread_run, (void*)message2);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
 
    printf("leakfinder C thread example app all done\n");
    return 0;
}
