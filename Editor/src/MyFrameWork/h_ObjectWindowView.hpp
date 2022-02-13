#ifndef __OBJECT_WINDOW_VIEW_HPP__
#define __OBJECT_WINDOW_VIEW_HPP__

#include <math.h>

class ObjectWindowView : public WindowControll
{
  public:
    ObjectWindowView(ImVec2, ImVec2, const char*,bool);
    ~ObjectWindowView();
    
    ImVec2 position;
    
    /* GameObject selectedGO; */
    
    void Draw() override;
    
    void DrawView();
    
};

#endif /* __OBJECT_WINDOW_VIEW_HPP__ */

