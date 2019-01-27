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

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})