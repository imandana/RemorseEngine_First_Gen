#ifndef __MEMORY_POOL_HPP__
#define __MEMORY_POOL_HPP__

#include <vector>

class MemoryPool
{
  public:
    MemoryPool(int);
    ~MemoryPool();
    
  void* RequestMemory(int);
    
/*   private: */
    char* memBuff;
    int size;
    
    std::vector<int> listSizeRequestedMem;
    std::vector<int> posMemoryPointer;
    int sumSize;
};

#endif /* __MEMORY_POOL_HPP__ */

