#ifndef __MENUBAR_CONTROLL_HPP__
#define __MENUBAR_CONTROLL_HPP__


class MenuBarControll : public ControllBase
{
  public:
    MenuBarControll();
    MenuBarControll(const char*,bool);
    ~MenuBarControll();
    
    std::vector<ControllBase*> cBaseView;
    
    void Draw() override;
    void AddControll( ControllBase* );
};

#endif /* __MENUBAR_CONTROLL_HPP__ */

