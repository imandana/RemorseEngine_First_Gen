
MemoryManager::MemoryManager()
{
    for(int i =0; i < Type::SIZE ; i++)
    {
        theMemory[i] = nullptr;
    }
}
MemoryManager::~MemoryManager()
{
    for(int i =0; i < Type::SIZE ; i++)
    {
        if( theMemory[i] != nullptr)
            delete theMemory[i];
    }
} 
void MemoryManager::InitializeMemoryOf( Type type, int size );
{
    /* Caution if memory already Assign, must delete it, carefull for the Data */
    if( theMemory[type] != nullptr )
    {
        delete theMemory[type];
    }
    
    MemoryPool* temp = new MemoryPool( size );
    
    theMemory[type].push_back ( temp );

}

MemoryPool* MemoryManager::GetMemoryPoolOf( Type type)
{
     return theMemory[ type ];
}
  
