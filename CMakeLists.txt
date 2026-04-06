cmake_minimum_required(VERSION 3.16)
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

# Main executable
add_executable(Nemu src/main.cpp src/core/Bus.cpp src/core/RP2A03.cpp src/utils/Disassembler.cpp)

# Testing
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)

option(NEMU_ENABLE_TESTS "Enable unit tests" ON) # Turn off with -DNEMU_ENABLE_TESTS=OFF
if(NEMU_ENABLE_TESTS)
    enable_testing()
    FetchContent_MakeAvailable(Catch2)
    add_subdirectory(tests)
endif()
