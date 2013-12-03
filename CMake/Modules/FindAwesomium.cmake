# Find the Awesomium library

find_path(AWESOMIUM_INCLUDE_DIRS
    Awesomium/WebCore.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(AWESOMIUM_LIBRARY
    NAMES awesomium
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")
