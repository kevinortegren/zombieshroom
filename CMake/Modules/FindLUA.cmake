# Find the LUA library

find_path(LUA_INCLUDE_DIRS
    lua/lua.hpp
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(LUA_LIBRARY
    NAMES lua
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
find_library(LUA_LIBRARY
    NAMES lua
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")