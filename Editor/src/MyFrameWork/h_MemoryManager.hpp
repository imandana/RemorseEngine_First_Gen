#ifndef __MEMORY_MANAGER_HPP__
#define __MEMORY_MANAGER_HPP__


class MemoryManager
{
  public:
    MemoryManager();
    ~MemoryManager();
    
/*   MemoryPool* memObjDynamic;
  MemoryPool* memResourceDynamic;
  MemoryPool* memObjStatic;
  MemoryPool* memResourceStatic; */
  MemoryPool* theMemory[ SIZE ];
  
  // Dynamic means. its recycled every Scene load. so the size of dynamic must the GREATER one between the scene
  // So, it must predefined size when build the game.
  enum Type {
      OBJ_DYNAMIC = 0,
      RES_DYNAMIC,
      OBJ_STATIC,
      RES_STATIC,
      SIZE
  }
  
  void InitializeMemoryOf( Type type );
  MemoryPool* GetMemoryPoolOf( Type type);
  
};

#endif /* __MEMORY_MANAGER_HPP__ */

