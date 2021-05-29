// reference https://www.codeproject.com/articles/393957/cplusplus-memory-leak-finder

#ifndef __allocation_info
#define __allocation_info

#include <vector>
#include <string>
#include <iostream>

#include <pthread.h>
using namespace std;

class allocation_info
{
    public:
      typedef long long address_type;

    private:
      allocation_info::address_type address;
      size_t size;
      vector<string> stacktrace;
      pthread_t thread_id;

    public:
      allocation_info(void* address, size_t size, char** stacktrace, size_t depth, pthread_t thread_id);

      allocation_info::address_type get_address() const;
      size_t get_size() const;
      vector<string> get_stacktrace() const;
      pthread_t get_thread_id() const;
};

#endif