#include "imgui.h"
#include <string>

// Windows-specific headers for system info
#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#endif

// Helper function to fetch the CPU Brand Name (Windows/x86/x64)
std::string GetCPUName() {
#ifdef _WIN32
    int cpuInfo[4] = { 0 };
    char cpuBrand[49] = { 0 };

    // Call __cpuid with instruction IDs that return the brand string
    __cpuid(cpuInfo, 0x80000002);
    memcpy(cpuBrand, cpuInfo, sizeof(cpuInfo));
    __cpuid(cpuInfo, 0x80000003);
    memcpy(cpuBrand + 16, cpuInfo, sizeof(cpuInfo));
    __cpuid(cpuInfo, 0x80000004);
    memcpy(cpuBrand + 32, cpuInfo, sizeof(cpuInfo));

    return std::string(cpuBrand);
#else
    return "Linux/macOS (Use /proc/cpuinfo or sysctl)";
#endif
}

// Helper function to fetch System RAM
float GetTotalRAM_GB() {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return static_cast<float>(memInfo.ullTotalPhys) / (1024 * 1024 * 1024);
#else
    return 0.0f;
#endif
}

// Call this inside your main rendering/update loop where ImGui frames are drawn
void ShowPCSpecsWindow(bool* p_open) {
    // Cache static specs so they are only queried once, not every frame
    static std::string cpuName = GetCPUName();
    static float totalRam = GetTotalRAM_GB();

    // Query your graphics API to get the GPU Name string
    // DX11: Using your IDXGIAdapter* adapter -> adapter->GetDesc(&desc); (desc.Description)
    // OpenGL: glGetString(GL_RENDERER)
    // Vulkan: VkPhysicalDeviceProperties.deviceName
    static std::string gpuName = "NVIDIA GeForce RTX 4070"; // Replace with your backend's string pointer

    ImGui::Begin("System Specifications", p_open, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextDisabled("Hardware Overview");
    ImGui::Separator();

    // Render specs using ImGui columns or text widgets
    ImGui::Text("CPU:");
    ImGui::SameLine(80);
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", cpuName.c_str());

    ImGui::Text("GPU:");
    ImGui::SameLine(80);
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%s", gpuName.c_str());

    ImGui::Text("RAM:");
    ImGui::SameLine(80);
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), "%.2f GB Installed", totalRam);

    ImGui::Separator();

    // Performance metrics directly from ImGui
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("App Performance: %.1f FPS (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);

    ImGui::End();
}
