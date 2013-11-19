# Find the SDL2 library

find_path(SDL2_INCLUDE_DIRS
    SDL2/SDL.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(SDL2_LIBRARY
    NAMES SDL2
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")