cmake_minimum_required(VERSION 3.10)
project(unicorn)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)

# --- PERBAIKAN: Perintah wajib agar VS Code IntelliSense tidak merah ---
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

add_executable(unicorn 
    src/main.cpp
    src/core/system_monitor.cpp
    src/core/database_manager.cpp
    src/ui/camera_view.cpp
    src/ui/multiview_camera.cpp
    src/ui/database_view.cpp
    src/ui/widget_gallery_view.cpp
    src/ui/sidebar_ui.cpp
    src/utils/graphics_utils.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(unicorn PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends 
    ${OpenCV_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${OpenCL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib  # Lokasi file .hpp Moildev
)

# --- CARA MODERN MEMUAT FILE .so DI CMAKE ---

# 1. Daftarkan libmoildev_ocl.so
add_library(moildev_ocl SHARED IMPORTED)
set_target_properties(moildev_ocl PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/x86_64/libmoildev_ocl.so")

# 2. Daftarkan libmoildev.so
add_library(moildev_core SHARED IMPORTED)
set_target_properties(moildev_core PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/x86_64/libmoildev.so")

# --- GABUNGKAN SEMUANYA ---
target_link_libraries(unicorn 
    ${OpenCV_LIBS}
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    OpenCL
    moildev_ocl    
    moildev_core   
    pthread
    dl
    sqlite3
)

# Trik RPATH agar aplikasi bisa jalan tanpa di-install ke sistem
set_target_properties(unicorn PROPERTIES
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/x86_64"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# --- CUSTOM TARGETS UNTUK BUILD & RUN ---
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/unicorn
    DEPENDS unicorn
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Moildev ImGui Application..."
)

add_custom_target(build-run
    COMMAND ${CMAKE_COMMAND} --build . --config Release
    COMMAND ${CMAKE_BINARY_DIR}/unicorn
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building and Running Moildev ImGui Application..."
)

add_custom_target(rebuild
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} --build . --config Release
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Clean Rebuild..."
)
