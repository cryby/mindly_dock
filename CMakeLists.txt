cmake_minimum_required(VERSION 3.26)
project(mindly)

set(CMAKE_CXX_STANDARD 20)

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

add_executable(mindly
        main.cpp
        ${CORE_SOURCES}
        ${IMGUI_SOURCES}
)

if(APPLE)
    set(LIBIMOBILEDEVICE_ROOT /opt/homebrew/opt/libimobiledevice)
    set(LIBPLIST_ROOT /opt/homebrew/opt/libplist)

    target_include_directories(mindly PRIVATE
            ${LIBIMOBILEDEVICE_ROOT}/include
            ${LIBPLIST_ROOT}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${IMGUI_DIR}
            ${IMGUI_DIR}/backends
            /opt/homebrew/include
    )

    target_link_directories(mindly PRIVATE
            ${LIBIMOBILEDEVICE_ROOT}/lib
            ${LIBPLIST_ROOT}/lib
    )

    target_link_libraries(mindly PRIVATE
            ${LIBIMOBILEDEVICE_ROOT}/lib/libimobiledevice-1.0.dylib
            ${LIBPLIST_ROOT}/lib/libplist-2.0.dylib
            glfw
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
elseif(WIN32)
    find_package(glfw3 REQUIRED)

    # Cesta k našim MinGW DLL
    set(VENDOR_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/win/libimobile/")

    target_include_directories(mindly PRIVATE
            # Hlavičky sú v gốc source, ale MinGW ich má niekedy v bin
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${IMGUI_DIR}
            ${IMGUI_DIR}/backends
    )

    # Linkuj presne tie, čo máš v zozname
    target_link_libraries(mindly PRIVATE
            "${VENDOR_BIN_DIR}/libimobiledevice-1.0.dll"
            "${VENDOR_BIN_DIR}/libplist-2.0.dll"
            "${VENDOR_BIN_DIR}/libusbmuxd-2.0.dll"
            glfw
            opengl32
    )

    # Skopíruj VŠETKY DLL k exe (vrátane závislostí)
    file(GLOB ALL_DLLS "${VENDOR_BIN_DIR}/*.dll")
    add_custom_command(TARGET mindly POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ALL_DLLS}
            $<TARGET_FILE_DIR:mindly>
            COMMENT "Kopírujem 30+ DLL z libimobile-suite..."
    )
endif()
