
MemoryPool::MemoryPool(int buffSize)
{
    size = buffSize;
    memBuff = (char*)malloc( buffSize );
    sumSize = 0;
}
MemoryPool::~MemoryPool()
{
    
}

void* MemoryPool::RequestMemory(int size)
{
    listSizeRequestedMem.push_back(size);
    posMemoryPointer.push_back( sumSize );
    sumSize += size;
    
    return static_cast<void*>( memBuff + posMemoryPointer[ posMemoryPointer.size() - 1 ] );
}
