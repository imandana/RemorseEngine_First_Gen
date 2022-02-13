
MenuBarControll::MenuBarControll()
: ControllBase() {}

MenuBarControll::MenuBarControll(const char* title, bool open = true)
: ControllBase(ImVec2(0,0) ,title,open)
{
}
MenuBarControll::~MenuBarControll()
{
    
}
void MenuBarControll::AddControll( ControllBase* cb)
{
    cBaseView.push_back( cb );
}
void MenuBarControll::Draw()
{
    if(ImGui::BeginMainMenuBar())
    {
            if(ImGui::BeginMenu("File"))
            {
                    if (ImGui::MenuItem("Open", "CTRL+O"))
                    {
                        
                    }
                    if (ImGui::MenuItem("Save", "CTRL+O"))
                    {
                        
                    }
                    if (ImGui::MenuItem("Exit", "CTRL+Q"))
                    {
                        isOpen = false;
                    }
                ImGui::EndMenu();
            }
            
            if(ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {
                    
                }
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            
            if(ImGui::BeginMenu("View"))
            {
                    for(int i=0; i< cBaseView.size(); i++)
                    {
                        if (ImGui::MenuItem( cBaseView[i]->title ))
                        {
                            cBaseView[i]->isOpen = !cBaseView[i]->isOpen;
                        }
                    }
                ImGui::EndMenu();
            }
            
            if(ImGui::BeginMenu("Build"))
            {
                if (ImGui::MenuItem("Windows")) {
                    
                }
                if (ImGui::MenuItem("Android")) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("WEBGL")) {}
                    if (ImGui::BeginMenu("PlayStation")) {
                        if (ImGui::MenuItem("PlayStation 1")) {}
                        if (ImGui::MenuItem("Playstation 2")) {}
                        if (ImGui::MenuItem("Playstation 3")) {}
                        if (ImGui::MenuItem("PSP (Portable)")) {}
                        if (ImGui::MenuItem("PS VITA")) {}
   
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Nintendo")) {         

                        if (ImGui::MenuItem("DS/3DS")) {}
                        if (ImGui::MenuItem("Wii")) {}
                        if (ImGui::MenuItem("Switch")) {}

                        ImGui::EndMenu();
                }
              
                if (ImGui::MenuItem("Symbian/Java Mobile")) {}
                ImGui::EndMenu();
            }
            
            
        ImGui::EndMainMenuBar();
    }
}
