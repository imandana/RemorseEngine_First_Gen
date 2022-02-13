#ifndef __CONTROLL_BASE_HPP__
#define __CONTROLL_BASE_HPP__

class ControllBase
{
  public:
    ControllBase();
    ControllBase(ImVec2 ,const char* , bool );
    virtual ~ControllBase();
    
    ImVec2 size;
    
    const char* title;
    
    virtual void Draw() = 0;
    
    bool isOpen;
    
};

#endif /* __CONTROLL_BASE_HPP__ */

