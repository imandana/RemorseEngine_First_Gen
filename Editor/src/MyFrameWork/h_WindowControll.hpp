#ifndef __WINDOW_CONTROLL_HPP__
#define __WINDOW_CONTROLL_HPP__


class WindowControll : public ControllBase
{
  public:
    WindowControll(ImVec2, ImVec2, const char*,bool);
    ~WindowControll();
    
    ImVec2 position;
    void Draw() override;
    
};

#endif /* __WINDOW_CONTROLL_HPP__ */

