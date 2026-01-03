cmake_minimum_required(VERSION 3.10)


project(Grep)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF) 

# 1. Enable high warnings
add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c++20)

# 2. (Optional but Recommended) Treat warnings as errors.
# This forces you to fix every single warning before you can run.
# add_compile_options(-Werror)

add_executable(main main.cpp)

# "Hey CMake, for the target 'main'..."
# "...PRIVATELY (just for itself)..."
# "...add the directory 'include' to its search path."

target_include_directories(main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")