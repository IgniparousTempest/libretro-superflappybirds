cmake_minimum_required(VERSION 3.7)

project(lr_superflappybirds)

set (CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} SHARED
        libretro.hpp
        libretro.cpp
        image_library.hpp
        game.cpp game.h
        pipe_pair.cpp pipe_pair.hpp
        textures.hpp
        bird.cpp bird.hpp
        input.hpp
        auxillary.hpp
        menu.cpp menu.hpp)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES})