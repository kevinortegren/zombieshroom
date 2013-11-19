# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(ROOT_PROJECT_NAME ZombieShroom)
project(${ROOT_PROJECT_NAME})

# TODO: Find external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

# TODO: Set global compiler options

# TODO: Add sub projects and dependencies. Compile root project.
add_subdirectory(RootEngine)
add_subdirectory(RootForce)
add_subdirectory(RootTools)