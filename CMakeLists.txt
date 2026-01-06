cmake_minimum_required(VERSION 3.10)


project(Grep)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF) 

# Fix for cross-platform working
if(MSVC)
    # Windows/MSVC specific flags
    # /W4 = High warning level (similar to -Wall -Wextra)
    # /WX = Treat warnings as errors (same as -Werror)
    # /std:c++20 = Use C++20 standard
    add_compile_options(/W4 /WX /std:c++20)
else()
    # Linux/Mac (GCC/Clang) specific flags
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c++20)
endif()

add_executable(main main.cpp handleInput.cpp grepEngine.cpp)

# "Hey CMake, for the target 'main'..."
# "...PRIVATELY (just for itself)..."
# "...add the directory 'include' to its search path."

target_include_directories(main  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")