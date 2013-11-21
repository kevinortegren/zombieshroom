# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(PROJECT_NAME RootEngine)
project(${PROJECT_NAME})

# Set the files used in the target
file(GLOB_RECURSE SOURCES Sources/ *.cpp)
file(GLOB_RECURSE HEADERS Headers/ *.h)
set(LIBRARIES ${EXTERNAL_LIBRARIES})

#message("${SOURCES}")
#message("${HEADERS}")

# Set search paths
include_directories(Include)

# TODO: Add the target (commented until we actually have a source file)
#add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
#target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
add_subdirectory(Network)
add_subdirectory(InputManager)