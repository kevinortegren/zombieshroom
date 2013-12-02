# Find the OpenGL Extension Wrangler library

find_path(GLEW_INCLUDE_DIRS
    GL/glew.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(GLEW_LIBRARY
    NAMES GLEW glew32
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")
	
find_library(GLEW_LIBRARY_64
	NAMES glew64
	HINTS "${CMAKE_SOURCE_DIR}/External/Lib") 