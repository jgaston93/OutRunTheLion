
cmake_minimum_required (VERSION 3.16.0)
project (OutRunTheLion)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(src)