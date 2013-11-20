# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(ROOT_PROJECT_NAME ZombieShroom)
project(${ROOT_PROJECT_NAME})

# TODO: Find external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)
find_package(GLEW REQUIRED)

set(EXTERNAL_INCLUDE_DIRS 
    ${SDL2_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)
    
set(EXTERNAL_LIBRARIES
    ${SDL2_LIBRARY}
    ${GTEST_LIBRARY}
    ${GLEW_LIBRARY}
)


# TODO: Set global compiler options
add_definitions(-D_VARIADIC_MAX=10)
add_definitions(-DUNICODE -D_UNICODE)

# TODO: Add sub projects and dependencies. Compile root project.
include_directories(${EXTERNAL_INCLUDE_DIRS})

add_subdirectory(RootEngine)
add_subdirectory(RootForce)
add_subdirectory(RootTools)