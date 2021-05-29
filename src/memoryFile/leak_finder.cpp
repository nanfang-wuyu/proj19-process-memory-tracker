#include<iostream>

void* malloc(size_t size)
{
  std::cout << "malloc called" << std::endl;
  // Somehow track this allocation
}

void free(void* ptr)
{
  std::cout << "free called" << std::endl;
  // Somehow marry up this deallocation to a previous allocation
}