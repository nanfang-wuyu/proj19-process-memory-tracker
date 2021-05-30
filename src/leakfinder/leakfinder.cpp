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
#include "allocation_func.cpp"

//using namespace bornander::memory;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

using namespace std;

static void compile_allocation() __attribute__((destructor));
static pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;

static size_t allocation_count = 0;
static vector<allocation_info> allocation_list;
static vector<thread> thread_list;

static const size_t max_frame_depth = 128;
static bool isExternalSource = true;
static void* (*sys_malloc)(size_t) = 0;
static void (*sys_free)(void*) = 0;
static void mutex_protect_start();
static void mutex_protect_end();
// void show_allocation(vector<allocation_info> allocation_list);

void show_thread(){
    cout << BOLDMAGENTA << setw(25) << "TID " << setw(15) << "size " << setw(5) <<"num" << RESET << endl;
    // cout << thread_list.size() << endl;
    for (size_t i = 0; i < thread_list.size(); i++)
    {
        /* code */
        cout << setw(25) << thread_list[i].thread_id << setw(9) << thread_list[i].total_leak_size << " Bytes" << setw(5) << thread_list[i].leak_num << endl;
    }
}

void show_allocation(){
    cout << BOLDMAGENTA << setw(25) << "TID" << setw(25) << "Address" << setw(15) << "Leak Size" << setw(5) << "" << "Stacktrace" << RESET << endl;
    for (size_t i = 0; i < allocation_list.size(); i++)
    {
        /* code */
        cout << setw(25) << allocation_list[i].get_thread_id() << setw(25) << allocation_list[i].get_address() << setw(9) << allocation_list[i].get_size() << " Bytes" << setw(5) << "" << flush;
        vector<string> stacktrace = allocation_list[i].get_stacktrace();
        for (size_t j = 0; j < stacktrace.size(); j++)
        {
            /* code */
            if (j == 0) {
                cout<< stacktrace[j]<<"\n"<<flush;
                continue;
            }
            cout<< setw(70) << "" << stacktrace[j]<<"\n"<<flush;
        }
        cout<<endl<<endl;
    }
    
}

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
    ifstream file("/home/sakura/project/mode.txt");
    string mode;
    file >> mode;
    file.close();
    if (allocation_list.empty())
    {
        cout << "leakfinder found no leaks, not one of the " << allocation_count;
        cout << "allocations was not released." << endl;
    }
    else
    {
        cout << "leakfinder detected that " << allocation_list.size();
        cout << " out of " << allocation_count << " allocations was not released." << endl;

        vector<allocation_info> tmp;
        
        for (int i = 0; i < allocation_list.size(); ++i)
        {
            allocation_info allocation = allocation_list[i];
            // cout << "Leak " << (i+1) << "@0x" << hex << allocation.get_thread_id() << dec;
            // cout << "; leaked " << allocation.get_size() << " bytes at position 0x";
            // cout << hex << allocation.get_address() << dec << endl;

            vector<string> stacktrace = allocation.get_stacktrace();
            // for (int j = 0; j < stacktrace.size(); ++j)
            // {
                // cout << "\t" << stacktrace[j] << endl;
            // }
            tmp = allocation_add(allocation_list, allocation, mode, true);
            thread_list = thread_add(thread_list, allocation, mode, true);
        }
        allocation_list = tmp;
    }
    
    cout << "\033c" << endl;
    show_thread();
    cout << BOLDGREEN << setfill('_') << setw(72) << RESET << endl;
    cout << setfill(' ') << endl;
    // cout << "" << endl;
    show_allocation();
    save_allocation(allocation_list, "/home/sakura/project/allocation.csv");
    save_thread(thread_list, "/home/sakura/project/thread.csv");
    // cout << mode <<endl;
    
}
