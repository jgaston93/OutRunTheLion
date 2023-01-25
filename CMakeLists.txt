
cmake_minimum_required (VERSION 3.16.0)
project (OutRunTheLion)

list(APPEND CMAKE_PREFIX_PATH "C:/Users/jgast/Documents/Source/GameDev/SDL2-2.26.1")
list(APPEND CMAKE_PREFIX_PATH "C:/Users/jgast/Documents/Source/GameDev/SDL2_image-2.6.2")
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)

add_subdirectory(src)