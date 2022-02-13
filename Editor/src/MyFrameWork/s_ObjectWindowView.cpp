
ObjectWindowView::ObjectWindowView(ImVec2 sz, ImVec2 pos, const char* title, bool open = true)
: position(pos), WindowControll(sz, pos, title, open)
{
}
ObjectWindowView::~ObjectWindowView()
{
    
}
void ObjectWindowView::Draw()
{
    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, &isOpen))
        {
            ImGui::End();
            return;
        }
    
    DrawView();
    
    ImGui::End();
}

void ObjectWindowView::DrawView()
{
       static int selected = 0;
        {
            ImGui::BeginChild(title, ImVec2(150, 0), true);
            for (int i = 0; i < 100; i++)
            {
                char label[128];
                sprintf(label, "GameObject %d", i);
                if (ImGui::Selectable(label, selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text("MyObject: %d", selected);
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Details"))
                {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            if (ImGui::Button("Revert")) {}
            ImGui::SameLine();
            if (ImGui::Button("Save")) {}
            ImGui::EndGroup();
        }
}

