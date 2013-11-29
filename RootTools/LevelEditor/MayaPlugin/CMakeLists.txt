# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(PROJECT_NAME MayaPlugin)
project(${PROJECT_NAME})

# Set output directory
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "C:/Program Files/Autodesk/Maya2014/bin/plug-ins/")

# Set the files used in the target
file(GLOB_RECURSE SOURCES Source/*.cpp)
file(GLOB_RECURSE HEADERS Include/*.h)
set(LIBRARIES ${MAYA_LIBRARIES})

# Set search paths
include_directories(Include)

# Add the target
add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})

# Copy the DLL on compile
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/Debug/MayaPlugin.dll" "C:/Program Files/Autodesk/Maya2014/bin/plug-ins/MayaPlugin.mll")