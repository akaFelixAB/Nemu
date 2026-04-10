cmake_minimum_required(VERSION 3.31)
project(Nemu LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Recommend g++
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GNU compiler: ${CMAKE_CXX_COMPILER}")
else()
    message(WARNING "Recommended compiler is g++.")
endif()

# Enable Ninja generator if available
if (CMAKE_GENERATOR STREQUAL "Ninja")
    message(STATUS "Ninja build system detected.")
endif()

include(FetchContent)

# ----------------------------
# SDL3
# ----------------------------
find_package(SDL3 REQUIRED)

# ----------------------------
# Dear ImGui
# ----------------------------
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.7
)
FetchContent_GetProperties(imgui)

FetchContent_MakeAvailable(imgui)

# ImGui static library
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC SDL3::SDL3)

# ----------------------------
# OpenGL
# ----------------------------
find_package(OpenGL REQUIRED)

# ----------------------------
# Main executable
# ----------------------------
add_executable(Nemu
    src/gui/gui_main.cpp 
    src/core/Bus.cpp 
    src/core/RP2A03.cpp 
    src/utils/Disassembler.cpp
)

# ----------------------------
# Testing
# ----------------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.14.0
)

option(NEMU_ENABLE_TESTS "Enable unit tests" ON) # Turn off with -DNEMU_ENABLE_TESTS=OFF
if (NEMU_ENABLE_TESTS)
    enable_testing()
    FetchContent_MakeAvailable(Catch2)
    add_subdirectory(tests)
endif()
