#ifndef __CONSOLE_WINDOW_HPP__
#define __CONSOLE_WINDOW_HPP__

#include <math.h>

class ConsoleWindow : public WindowControll
{
  public:
    ConsoleWindow(ImVec2, ImVec2, const char*,bool);
    ~ConsoleWindow();
    
    ImVec2 position;
    
    /* GameObject selectedGO; */
    
    void Draw() override;
    
    void DrawView();
    
};

#endif /* __OBJECT_WINDOW_VIEW_HPP__ */


