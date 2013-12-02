# Find the YAML library.

find_path(YAML_INCLUDE_DIRS
    yaml-cpp/yaml.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(YAML_LIBRARY
    NAMES libyaml-cppmd
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/Release")

find_library(YAML_LIBRARY_DEBUG
    NAMES libyaml-cppmdd
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/Debug")
	
find_library(YAML_LIBRARY_DEBUG_64
    NAMES libyaml-cppmdd64
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/Debug")
