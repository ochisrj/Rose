#include "menubar.h"

void MenuBar::Draw(GLFWwindow * window)
{
    if (ImGui::BeginMainMenuBar())
    {
        FileMenu::DrawMenu();
        ViewMenu::DrawMenu();
        MeshMenu::DrawMenu();

        ImGui::SameLine();
        ImGui::Text(" | %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }
}
