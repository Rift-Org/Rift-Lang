cmake_minimum_required(VERSION 3.0)
project(RiftLang)

# C++17 for now might switch to 20
set(CMAKE_CXX_STANDARD 17)

# sets targets
include(Options.mk)

# "#includes"
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external)

add_subdirectory(lib)
enable_testing()
add_subdirectory(tests)