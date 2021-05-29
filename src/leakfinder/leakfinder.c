#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
     
#define __USE_GNU
#include <dlfcn.h>

class allocation_info
{
private:
 
public:
};

static bool isExternalSource=true;
static void* (*real_malloc)(size_t)=0;
static void (*real_free)(void*)=0;

static void __mtrace_init(void)
{
    real_malloc = (void*(*)(size_t))dlsym(RTLD_NEXT, "malloc");
    if (real_malloc == 0) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
        return;
    }

    real_free = (void(*)(void*))dlsym(RTLD_NEXT, "free");
    if (real_free == 0) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
        return;
    }


}

extern "C" void *malloc(size_t size)
{
    if(real_malloc==0)
        __mtrace_init();

    void *p = real_malloc(size);

    if (isExternalSource)
    {
        isExternalSource = false;

        printf("Allocating %d bytes\n", size);        

        isExternalSource = true;
    }	

    return p;
}

extern "C" void free(void* ptr) 
{
    if(real_free==0)
        __mtrace_init();

    int address = (int)ptr;
    real_free(ptr);

    if (isExternalSource)
    {
        isExternalSource = false;
        printf("Freed memory at %d \n", address);        
        isExternalSource = true;
    }
}
