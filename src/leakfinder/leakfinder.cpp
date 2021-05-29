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
// #include "./memoryFile/allocation_func.cpp"

//using namespace bornander::memory;
using namespace std;

static void compile_allocation() __attribute__((destructor));
static pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;

static size_t allocation_count = 0;
static vector<allocation_info> allocation_list;
// static vector<thread> thread_list;

static const size_t max_frame_depth = 128;
static bool isExternalSource = true;
static void* (*sys_malloc)(size_t) = 0;
static void (*sys_free)(void*) = 0;
static void mutex_protect_start();
static void mutex_protect_end();

static void initialize_functions(void)
{
    sys_malloc = reinterpret_cast<void*(*)(size_t)>(dlsym(RTLD_NEXT, "malloc"));
    if (sys_malloc == nullptr)
        cerr << "Leak finder failed to read malloc function; " << dlerror() << endl;

    sys_free = reinterpret_cast<void(*)(void*)>(dlsym(RTLD_NEXT, "free"));
    if (sys_free == nullptr)
        cerr << "Leak finder failed to read free function; " << dlerror() << endl;
}

extern "C" void* malloc(size_t size)
{
    if (sys_malloc == nullptr)
        initialize_functions();

    void* ptr = sys_malloc(size);

    if (isExternalSource)
    {
        pthread_t thread_id = pthread_self();

        mutex_protect_start();

        ++allocation_count;

        void* frames[max_frame_depth];
        size_t stack_size = backtrace(frames, max_frame_depth);
        char** stacktrace = backtrace_symbols(frames, stack_size);
        allocation_info allocation(ptr, size, stacktrace, stack_size, thread_id);

        allocation_list.push_back(allocation);

        sys_free(stacktrace);

//        cout << "malloc thread " << thread_id << endl;
        mutex_protect_end();
    }

    return ptr;
}

extern "C" void free(void* ptr)
{
    if (sys_free == nullptr)
        initialize_functions();

    allocation_info::address_type address = reinterpret_cast<allocation_info::address_type>(ptr);
    sys_free(ptr);

    if (isExternalSource)
    {
        mutex_protect_start();
        for (int i = 0; i < allocation_list.size(); ++i)
        {
            allocation_info allocation = allocation_list[i];
            if (allocation.get_address() == address)
            {
                allocation_list.erase(allocation_list.begin() + i);
                break;
            }
        }
//        cout << "free address " << address << endl;
        mutex_protect_end();
    }
}

static void mutex_protect_start(){
    pthread_mutex_lock(&cs_mutex);
    isExternalSource = false;
}

static void mutex_protect_end(){
    isExternalSource = true;
    pthread_mutex_unlock(&cs_mutex);
}

void compile_allocation()
{
    isExternalSource = false;
    if (allocation_list.empty())
    {
        cout << "leakfinder found no leaks, not one of the " << allocation_count;
        cout << " allocations was not released." << endl;
    }
    else
    {
        cout << "leakfinder detected that " << allocation_list.size();
        cout << " out of " << allocation_count << " allocations was not released." << endl;
        for (int i = 0; i < allocation_list.size(); ++i)
        {
            allocation_info allocation = allocation_list[i];
            cout << "Leak " << (i+1) << "@0x" << hex << allocation.get_thread_id() << dec;
            cout << "; leaked " << allocation.get_size() << " bytes at position 0x";
            cout << hex << allocation.get_address() << dec << endl;

            vector<string> stacktrace = allocation.get_stacktrace();
            for (int j = 0; j < stacktrace.size(); ++j)
            {
                cout << "\t" << stacktrace[j] << endl;
            }
            // allocation_add(allocation_list, allocation, "");
        }
    }
}
