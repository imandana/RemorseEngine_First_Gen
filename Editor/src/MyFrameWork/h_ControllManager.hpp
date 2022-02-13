#ifndef __CONTROLL_MANAGER_HPP__
#define __CONTROLL_MANAGER_HPP__

#include <vector>

class ControllManager
{
  public:
    ControllManager();
    ~ControllManager();
    
    void RegisterControll(ControllBase*);
    void DrawControll();
    const std::vector<ControllBase*>* GetAllListControll();
    
  private:
    std::vector<ControllBase*> listControll;
    
};

#endif /* __CONTROLL_MANAGER_HPP__ */

