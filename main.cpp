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


    while (!glfwWindowShouldClose(gui_manager.get_window())) {
        gui_manager.begin();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoBackground;

        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 5.0f;
        style.FramePadding = ImVec2(10, 5);

        ImGui::Begin("Mindly App", NULL, window_flags);
        int height, width;
        glfwGetWindowSize(gui_manager.get_window(), &width, &height);
        ImGui::SetCursorPos(ImVec2(width/2 - ImGui::CalcTextSize("Not connected").x/2, 30));

        if (device_manager.get_connected()) {
            ImGui::Text(device_manager.get_device_name().c_str());
            if (!pm.is_installed) {
                ImGui::SetCursorPos(ImVec2(width/2 - ImGui::CalcTextSize("Enable Dumb Mode").x/2-5, height/2));
                if (ImGui::Button("Enable Dumb Mode")) {
                    if (!pm.install_profile("")) {
                        std::cout << "Failed to install profile" << std::endl;
                    }
                }
            } else {
                ImGui::SetCursorPosX(width/2 - ImGui::CalcTextSize("ACTION REQUIRED ON IPHONE:").x/2);
                ImGui::TextColored(ImVec4(1,1,0,1), "ACTION REQUIRED ON IPHONE:");
                ImGui::SetCursorPosX(20);
                ImGui::BulletText("1. Open Settings");
                ImGui::SetCursorPosX(20);
                ImGui::BulletText("2. Tap 'Profile Downloaded' (Top)");
                ImGui::SetCursorPosX(20);
                ImGui::BulletText("3. Click 'Install'");
                ImGui::SetCursorPosX(width/2 - ImGui::CalcTextSize("Disable Dumb Mode").x/2-5);
                if (ImGui::Button("Disable Dumb Mode")) {
                    if (!pm.remove_profile("com.mindly.restriction")) {
                        std::cout << "Failed to remove profile" << std::endl;
                    }
                }
            }
        } else {
            ImGui::Text("Not Connected");
        }

        ImGui::SetCursorPos(ImVec2(200 - ImGui::CalcTextSize("v0.1 (Beta)").x/2+30, 175));
        ImGui::TextDisabled("v0.1 (Beta)");
        ImGui::End();


        gui_manager.end();
    }

    gui_manager.destroy();
    return 0;
}
