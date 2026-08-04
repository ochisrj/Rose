#include "view.h"
#include "imgui.h"
#include "config_control.h"
#include "bechmark.h"
#include "camera.h"

extern Camera camera;

static bool showViewWindow = true;

void ViewMenu::DrawMenu()
{
    if (ImGui::BeginMenu("View"))
    {
        ImGui::MenuItem("Control Panel", NULL, &ConfigControl::showMyWindow);
        ImGui::MenuItem("Benchmark Window", NULL, &BenchMark::Visible);
        ImGui::Separator();
        ImGui::MenuItem("Wireframe Mode", NULL, &ConfigControl::wireframemode);
        ImGui::MenuItem("Show 10 Cubes", NULL, &ConfigControl::showMultipleCubes);
        ImGui::MenuItem("Auto Rotate (Y-Axis)", NULL, &ConfigControl::autoRotate);
        ImGui::MenuItem("View Window", NULL, &showViewWindow);
        ImGui::Separator();
        if (ImGui::MenuItem("Reset Camera Position"))
        {
            camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
            camera.Yaw = -90.0f;
            camera.Pitch = 0.0f;
        }
        ImGui::EndMenu();
    }
}

void ViewMenu::DrawWindow()
{
    if (!showViewWindow) return;

    if (ImGui::Begin("View", &showViewWindow))
    {
        ImGui::SeparatorText("Model View Control");

        ImGui::SliderFloat("FOV (Zoom)", &camera.Zoom, 1.0f, 90.0f);
        ImGui::SliderFloat("Camera Speed", &camera.MovementSpeed, 0.5f, 15.0f);
        ImGui::SliderFloat("Mouse Sensitivity", &camera.MouseSensitivity, 0.01f, 0.5f);

        ImGui::Checkbox("Wireframe", &ConfigControl::wireframemode);
        ImGui::Checkbox("Show 10 Cubes", &ConfigControl::showMultipleCubes);
        ImGui::Checkbox("Auto Rotate (Y)", &ConfigControl::autoRotate);

        if (!ConfigControl::autoRotate)
        {
            ImGui::SliderFloat("Rotate X", &ConfigControl::rotateX, 0.0f, 360.0f);
            ImGui::SliderFloat("Rotate Y", &ConfigControl::rotateY, 0.0f, 360.0f);
            ImGui::SliderFloat("Rotate Z", &ConfigControl::rotateZ, 0.0f, 360.0f);
        }

        if (ImGui::Button("Reset Camera"))
        {
            camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
            camera.Yaw = -90.0f;
            camera.Pitch = 0.0f;
        }
    }
    ImGui::End();
}
