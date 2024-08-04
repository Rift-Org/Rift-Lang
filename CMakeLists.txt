cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0079 NEW)

# Built using Clang 🐐
set(CMAKE_CXX_COMPILER_ID "Clang")
set(CMAKE_C_COMPILER_ID "Clang")

# Find the Clang compiler
find_program(CMAKE_CXX_COMPILER NAMES clang++ clang++-14 clang++-13 clang++-12 clang++-11 clang++-10 clang++-9 clang++-8 clang++-7 clang++-6.0 clang++)
find_program(CMAKE_C_COMPILER NAMES clang clang-14 clang-13 clang-12 clang-11 clang-10 clang-9 clang-8 clang-7 clang-6.0 clang)

# Double Check
if(NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "Clang++ not found")
endif()
if(NOT CMAKE_C_COMPILER)
    message(FATAL_ERROR "Clang not found")
endif()

project(RiftLang)

# C++20 required
set(CMAKE_CXX_STANDARD 20)
# Set Debug Mode
set(CMAKE_BUILD_TYPE Debug)
# add -DCMAKE_EXPORT_COMPILE_COMMANDS=ON to generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# sets targets
include(Options.mk)

# Add the directory containing FindReadline.cmake to the module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# Find the Readline package
find_package(Readline REQUIRED)

# "#includes"
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external)

# Add subdirectories and define targets
add_subdirectory(lib)

# Now that the target is defined, we can link Readline
if(READLINE_FOUND)
    target_include_directories(riftlang PRIVATE ${Readline_INCLUDE_DIR})
    target_link_libraries(riftlang PRIVATE ${Readline_LIBRARY})
else()
    message(FATAL_ERROR "Readline not found")
endif()

enable_testing()
add_subdirectory(tests)

# Google {Test&Mock}
add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

# Debug output
message(STATUS "Readline include dir: ${Readline_INCLUDE_DIR}")
message(STATUS "Readline library: ${Readline_LIBRARY}")