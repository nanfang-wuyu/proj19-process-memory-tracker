#include "allocation_info.hpp"

namespace bornander
{
  namespace memory
  {
    allocation_info::allocation_info(void* address, size_t size, char** stacktrace, size_t depth, pthread_t thread_id)
    {
      this->address = reinterpret_cast<allocation_info::address_type>(address);
      this->size = size;
      this->thread_id = thread_id;

      // Skip first frame as that is the overriden malloc method
      for(int i = 1; i < depth; ++i)
      {
        std::string frame = stacktrace[i];
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

    std::vector<std::string> allocation_info::get_stacktrace() const
    {
      return stacktrace;
    }

    pthread_t allocation_info::get_thread_id() const
    {
      return thread_id;
    }
  }
}