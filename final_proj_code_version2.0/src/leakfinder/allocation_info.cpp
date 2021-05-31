// reference https://www.codeproject.com/articles/393957/cplusplus-memory-leak-finder

#include "allocation_info.hpp"

allocation_info::allocation_info(void* address, size_t size, char** stacktrace, size_t depth, pthread_t thread_id)
{
    this->address = reinterpret_cast<allocation_info::address_type>(address);
    this->size = size;
    this->thread_id = thread_id;
    
    // Skip first frame as that is the overriden malloc method
    for(int i = 0; i < depth; ++i)
    {
        string frame = stacktrace[i];
        this->stacktrace.push_back(frame);
        
    }
    
}

allocation_info::address_type allocation_info::get_address() const
{
    return address;
}

size_t allocation_info::get_size() const
{
    return size;
}

vector<string> allocation_info::get_stacktrace() const
{
    return stacktrace;
}

pthread_t allocation_info::get_thread_id() const
{
    return thread_id;
}


