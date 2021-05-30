#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <execinfo.h>
#include <pthread.h>
     
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <dlfcn.h>

#include "allocation_info.hpp"

using namespace bornander::memory;
using namespace std;

static void compile_allocation() __attribute__((destructor));

static pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;

static size_t allocation_count = 0;
static vector<allocation_info> allocations;

static const size_t max_frame_depth = 128;
static bool isExternalSource = true;
static void* (*sys_malloc)(size_t) = 0;
static void (*sys_free)(void*) = 0;

static void initialize_functions(void)
{
    sys_malloc = reinterpret_cast<void*(*)(size_t)>(dlsym(RTLD_NEXT, "malloc"));
    if (sys_malloc == 0) 
        cerr << "leakfinder failed to read malloc function; " << dlerror() << endl; 

    sys_free = reinterpret_cast<void(*)(void*)>(dlsym(RTLD_NEXT, "free"));
    if (sys_free == 0) 
        cerr << "leakfinder failed to read free function; " << dlerror() << endl; 
}

extern "C" void* malloc(size_t size)
{
    if (sys_malloc == 0)
        initialize_functions();

    void* ptr = sys_malloc(size);

    if (isExternalSource)
    {
        pthread_t thread_id = pthread_self();
        pthread_mutex_lock(&cs_mutex);
        isExternalSource = false;
        
        ++allocation_count;
        
        void* frames[max_frame_depth];
        size_t stack_size = backtrace(frames, max_frame_depth);
        char** stacktrace = backtrace_symbols(frames, stack_size);
        allocation_info allocation(ptr, size, stacktrace, stack_size, thread_id);

        allocations.push_back(allocation);
     
        sys_free(stacktrace);

        isExternalSource = true;

        pthread_mutex_unlock(&cs_mutex);
    }	

    return ptr;
}

extern "C" void free(void* ptr) 
{
    if (sys_free == 0)
        initialize_functions();

    allocation_info::address_type address = reinterpret_cast<allocation_info::address_type>(ptr);
    sys_free(ptr);

    if (isExternalSource)
    {
        pthread_mutex_lock(&cs_mutex);

        isExternalSource = false;
        for (int i = 0; i < allocations.size(); ++i)
        {
            allocation_info allocation = allocations[i];
            if (allocation.get_address() == address)
            {
                allocations.erase(allocations.begin() + i);
                break;
            }        
        }
        isExternalSource = true;

        pthread_mutex_unlock(&cs_mutex);
    }
}

void compile_allocation()
{
    isExternalSource = false; 
    if (allocations.empty())
    {
        cout << "leakfinder found no leaks, not one of the " << allocation_count;
        cout << " allocations was not released." << endl;
    }
    else
    {
        cout << "leakfinder detected that " << allocations.size();
        cout << " out of " << allocation_count << " allocations was not released." << endl;
        for (int i = 0; i < allocations.size(); ++i)
        {
            allocation_info allocation = allocations[i];
            cout << "Leak " << (i+1) << "@0x" << hex << allocation.get_thread_id() << dec;
            cout << "; leaked " << allocation.get_size() << " bytes at position 0x"; 
            cout << hex << allocation.get_address() << dec << endl; 
            
            vector<string> stacktrace = allocation.get_stacktrace();
            for (int j = 0; j < stacktrace.size(); ++j)
            {
                cout << "\t" << stacktrace[j] << endl;
            } 
        } 
    }
}
