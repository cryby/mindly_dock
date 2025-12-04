cmake_minimum_required(VERSION 3.26)
project(mindly)

set(CMAKE_CXX_STANDARD 20)

set(LIBIMOBILEDEVICE_ROOT /opt/homebrew/opt/libimobiledevice)
set(LIBPLIST_ROOT /opt/homebrew/opt/libplist)

find_package(glfw3 REQUIRED)

set(IMGUI_DIR libs/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)


set(CORE_SOURCES
    src/device_manager/device_manager.cpp
    src/device_manager/device_manager.h
    src/profile_manager/profile_manager.cpp
    src/profile_manager/profile_manager.h
        src/gui_manager/gui_manager.cpp
        src/gui_manager/gui_manager.h
)

add_executable(mindly MACOSX_BUNDLE
    main.cpp
    ${CORE_SOURCES}
    ${IMGUI_SOURCES}
)

include_directories(
    ${LIBIMOBILEDEVICE_ROOT}/include
    ${LIBPLIST_ROOT}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    /opt/homebrew/include
)

link_directories(
    ${LIBIMOBILEDEVICE_ROOT}/lib
    ${LIBPLIST_ROOT}/lib
)

target_link_libraries(mindly PRIVATE
    /opt/homebrew/opt/libimobiledevice/lib/libimobiledevice-1.0.dylib
    /opt/homebrew/opt/libplist/lib/libplist-2.0.dylib
    glfw
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
)
