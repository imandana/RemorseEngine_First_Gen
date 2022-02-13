
WindowControll::WindowControll(ImVec2 sz, ImVec2 pos, const char* title, bool open = true)
: position(pos), ControllBase(sz,title,open)
{
}
WindowControll::~WindowControll()
{
    
}
void WindowControll::Draw()
{
    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, &isOpen))
        {
            ImGui::End();
            return;
        }
        ImGui::End();
}
