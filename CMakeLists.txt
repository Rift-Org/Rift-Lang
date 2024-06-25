cmake_minimum_required(VERSION 3.10)

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

# C++17 required
set(CMAKE_CXX_STANDARD 20)

# Set Debug Mode
set(CMAKE_BUILD_TYPE Debug)

# add -DCMAKE_EXPORT_COMPILE_COMMANDS=ON to generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# sets targets
include(Options.mk)
include(FindReadline.mk)

# "#includes"
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external)

add_subdirectory(lib)
enable_testing()
add_subdirectory(tests)

# Google {Test&Mock}
add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

# Google {Abseil}
# add_compile_definitions(ABSL_USE_GOOGLE_DEFAULT_NAMESPACE)
# add_subdirectory(external/abseil)
# find_package(absl REQUIRED)
# include_directories(${abseil_SOURCE_DIR})
# target_link_libraries(riftlang absl::base absl::synchronization absl::strings)

#readline library for interpreter
# find_package(Readline REQUIRED)
# if READLINE_FOUND is false then fail
if(NOT READLINE_FOUND)
    message(FATAL_ERROR "Readline not found")
endif()