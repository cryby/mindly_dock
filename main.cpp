#include <fstream>
#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

#include "imgui_impl_opengl3.h"
#include "gui_manager/gui_manager.h"
#include "src/device_manager/device_manager.h"
#include "src/profile_manager/profile_manager.h"

int main() {

    gui_manager gui_manager;
    gui_manager.init();

    device_manager device_manager;
    auto pm = profile_manager(device_manager);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;

    while (!glfwWindowShouldClose(gui_manager.get_window())) {
        gui_manager.begin();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Mindly App", NULL, window_flags);
        int height, width;
        glfwGetWindowSize(gui_manager.get_window(), &width, &height);
        ImGui::SetCursorPos(ImVec2(width/2 - ImGui::CalcTextSize("Not connected").x/2, 50));

        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF("/Users/tobiaskocur/CLionProjects/mindly/fonts/sf.ttf", 16.f, NULL);
        ImGui::PushFont(font);

        if (device_manager.get_connected()) {
            if (ImGui::Button("Disconnect")) {
                device_manager.disconnect();
            }
            if (!pm.is_installed) {
                if (ImGui::Button("Enable Dumb Mode")) {
                    if (!pm.install_profile("")) {
                        std::cout << "Failed to install profile" << std::endl;
                    }
                }
            } else {
                ImGui::Text(device_manager.get_device_name().c_str());
                ImGui::TextColored(ImVec4(1,1,0,1), "ACTION REQUIRED ON IPHONE:");
                ImGui::BulletText("1. Open Settings");
                ImGui::BulletText("2. Tap 'Profile Downloaded' (Top)");
                ImGui::BulletText("3. Click 'Install'");
                if (ImGui::Button("Disable Dumb Mode")) {
                    if (!pm.remove_profile("com.mindly.restriction")) {
                        std::cout << "Failed to remove profile" << std::endl;
                    }
                }
            }
        } else {
            ImGui::Text("Not Connected");
            ImGui::SetCursorPos(ImVec2(width/2 - 55, 80));
            if (ImGui::Button("Connect", ImVec2(100, 20))) {
                device_manager.connect();
            }
        }

        ImGui::PopFont();
        ImGui::End();


        gui_manager.end();
    }

    gui_manager.destroy();
    return 0;
}
