# Find the FMod library

find_path(FMOD_INCLUDE_DIRS
    FMod/fmod.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(FMOD_LIBRARY_DEBUG
    NAMES fmodexL_vc
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
	
find_library(FMOD_LIBRARY
    NAMES fmodex_vc
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")
	
find_library(FMOD_LIBRARY_64
    NAMES fmodexL64_vc
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")