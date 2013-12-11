# CMake settings
cmake_minimum_required(VERSION 2.8)

# Include common headers
include_directories(Include)

# Find Maya library
find_package(Maya REQUIRED)

# Add subdirectories for tools
add_subdirectory(3DViewer)
add_subdirectory(MayaPlugin)