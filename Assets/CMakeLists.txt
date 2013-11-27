# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(PROJECT_NAME _Assets)
project(${PROJECT_NAME})

# Set the files used in the target
file(GLOB_RECURSE SHADERS Shaders/*)
file(GLOB_RECURSE SCRIPTS Scripts/*)
file(GLOB_RECURSE GUI GUI/*)

# Add the target

add_custom_target(${PROJECT_NAME} ALL
	SOURCES ${SHADERS}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/Assets)

source_group("Shaders" FILES ${SHADERS})
source_group("Scripts" FILES ${SCRIPTS})
source_group("GUI" FILES ${GUI})