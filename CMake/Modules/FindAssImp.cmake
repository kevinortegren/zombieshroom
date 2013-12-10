# Find the Assimp library

find_path(ASSIMP_INCLUDE_DIRS
    assimp/mesh.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(ASSIMP_LIBRARY
    NAMES assimp
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86")