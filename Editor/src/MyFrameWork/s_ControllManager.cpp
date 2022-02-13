
ControllManager::ControllManager()
{
    /* Hardcoded for now */
    listControll.reserve(200);
}
ControllManager::~ControllManager()
{
    /* Manual Call Destructor */
    for(int i = 0 ; i< listControll.size(); i++)
         listControll[i]->~ControllBase();
    
    listControll.empty();
}

void ControllManager::RegisterControll(ControllBase* cb)
{
    listControll.push_back( cb );
}
void ControllManager::DrawControll()
{
    for(int i = 0 ; i< listControll.size(); i++)
    {
        if(listControll[i]->isOpen)
            listControll[i]->Draw();
    }
}

const std::vector<ControllBase*>* ControllManager::GetAllListControll()
{
    return &listControll;
}

