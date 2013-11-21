# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(ROOT_PROJECT_NAME ZombieShroom)
project(${ROOT_PROJECT_NAME})

# Find external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)
find_package(GLEW REQUIRED)
find_package(RakNet REQUIRED)
find_package(Assimp REQUIRED)

set(EXTERNAL_INCLUDE_DIRS 
    ${SDL2_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${RAKNET_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
)
    
set(EXTERNAL_LIBRARIES
    ${SDL2_LIBRARY}
    ${GTEST_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${RAKNET_LIBRARY}
    ${ASSIMP_LIBRARY}
)

# Set global compiler options
add_definitions(-D_VARIADIC_MAX=10)
add_definitions(-DUNICODE -D_UNICODE)

# Add sub projects and dependencies.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${EXTERNAL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(RootEngine)
add_subdirectory(RootForce)
add_subdirectory(RootTools)
add_subdirectory(Utility)
add_subdirectory(Assets)

# Copy the runtime dependencies to the output directory when dependency target is built
# TODO: This is hardcoded to the Debug folder. We cannot use a Release build...
add_custom_target(_Dependencies ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/External/Bin/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
)
 
