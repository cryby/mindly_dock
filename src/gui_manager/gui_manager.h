//
// Created by Tobias Kocúr on 04/12/2025.
//

#ifndef MINDLY_GUI_MANAGER_H
#define MINDLY_GUI_MANAGER_H

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <string>

class gui_manager {
    private:
        GLFWwindow* window{nullptr};

    public:
        gui_manager() = default;
        void destroy() const;
        void init();

        void begin();
        void end() const;
        GLFWwindow* get_window() const { return window; };

private:
    static void glfw_error_callback(int error, const char* description);
};


#endif //MINDLY_GUI_MANAGER_H