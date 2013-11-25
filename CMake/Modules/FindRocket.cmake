# Find the Rocket library

find_path(ROCKET_INCLUDE_DIRS
    Rocket/Core.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(ROCKET_CONTROLS_LIBRARY
    NAMES RocketControls
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")
	
find_library(ROCKET_CORE_LIBRARY
    NAMES RocketCore
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")
	
find_library(ROCKET_DEBUGGER_LIBRARY
    NAMES RocketDebugger
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")