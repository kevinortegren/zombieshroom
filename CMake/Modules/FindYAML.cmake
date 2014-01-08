# Find the YAML library.

find_path(YAML_INCLUDE_DIRS
    yaml-cpp/yaml.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")
	
find_library(YAML_LIBRARY_DEBUG
    NAMES libyaml-cppmdd
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
	
find_library(YAML_LIBRARY
    NAMES libyaml-cppmd
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")
	
find_library(YAML_LIBRARY_DEBUG_64
    NAMES libyaml-cppmdd64
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x64/Debug")
	
find_library(YAML_LIBRARY_64
    NAMES libyaml-cppmd64
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x64/Release")