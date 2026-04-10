# CMake configuration for tests
cmake_minimum_required(VERSION 3.31)

add_executable(test_main 
    test_main.cpp 
    ../src/core/Bus.cpp 
    ../src/core/RP2C02.cpp 
    ../src/core/RP2A03.cpp 
    ../src/utils/Disassembler.cpp)

# Link Catch2 to the test executable
target_link_libraries(test_main PRIVATE Catch2::Catch2WithMain)

# Register the test with CTest
add_test(NAME NemuTests COMMAND test_main)
