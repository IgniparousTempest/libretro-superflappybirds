cmake_minimum_required(VERSION 3.13)

project(lr_superflappybirds)

set (CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} SHARED
        libretro.h
        libretro.cpp
        image_library.h
        game.cpp game.h
        pipe_pair.cpp pipe_pair.h
        textures.h
        bird.cpp bird.h
        input.h
        auxillary.h
        menu.cpp menu.h)

INCLUDE(FindPkgConfig)

link_directories("C:\\Users\\Courts\\CLionProjects\\SDL2-2.0.9\\i686-w64-mingw32\\lib")
INCLUDE_DIRECTORIES("C:\\Users\\Courts\\CLionProjects\\SDL2-2.0.9\\i686-w64-mingw32\\include\\SDL2")
TARGET_LINK_LIBRARIES(${PROJECT_NAME} SDL2)