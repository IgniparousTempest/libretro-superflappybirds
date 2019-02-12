cmake_minimum_required(VERSION 3.7)

project(superflappybirds_libretro)

set (CMAKE_CXX_STANDARD 14)

# Enable OpenMP for multithreading in Engine
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    ENDIF()
ENDIF()

# Include engine files
file(GLOB ENGINE_SOURCE "engine/*.hpp" "engine/*.cpp")

# Set source files
add_library(${PROJECT_NAME} SHARED
        libretro.hpp
        libretro.cpp
        game.cpp game.hpp
        pipe_pair.cpp pipe_pair.hpp
        assets.hpp
        bird.cpp bird.hpp
        input.hpp
        auxillary.hpp
        menu.cpp menu.hpp
        save_data.cpp save_data.hpp
        highscore_sub_window.hpp
        contexts/context.hpp
        contexts/context_highscore_input.hpp contexts/context_highscore_input.cpp
        ${ENGINE_SOURCE})

target_link_libraries(${PROJECT_NAME})

# Removes the 'lib' prefix on Linux
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")