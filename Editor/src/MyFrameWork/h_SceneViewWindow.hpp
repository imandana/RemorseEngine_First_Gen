#ifndef __SCENE_VIEW_WINDOW_HPP__
#define __SCENE_VIEW_WINDOW_HPP__


class SceneViewWindow : public WindowControll
{
  public:
    SceneViewWindow(ImVec2, ImVec2, const char*,bool);
    ~SceneViewWindow();
    
    ImVec2 position;
 
    char objName[20];
 
    Entity testGambar;
    unsigned int testTexture;
    /* GameObject selectedGO; */
    ImVector<Component*> listComponent;
    
    
    ImVector<ImVec2> points;
    ImVec2 scrolling = {0.0f, 0.0f};
    bool opt_enable_grid = true;
    bool opt_enable_context_menu = true;
    bool adding_line = false;
    
    
    void Draw() override;
    
    void DrawFromSFML();
    void DrawCanvas();
    
};

#endif /* __SCENE_VIEW_WINDOW_HPP__ */

