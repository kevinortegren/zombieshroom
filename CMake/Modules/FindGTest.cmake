# Find the Google Test library

find_path(GTEST_INCLUDE_DIRS
    gtest/gtest.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(GTEST_LIBRARY
    NAMES gtest
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")