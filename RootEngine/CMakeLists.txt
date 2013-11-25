# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(PROJECT_NAME RootEngine)
project(${PROJECT_NAME})

# Set the files used in the target
file(GLOB_RECURSE SOURCES Source/*.cpp)
file(GLOB_RECURSE HEADERS Include/*.h)
set(LIBRARIES ${EXTERNAL_LIBRARIES} DynamicLoader)

#message("${SOURCES}")
#message("${HEADERS}")

# Set search paths
include_directories(Include)



# Add the target
add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Add subdirectories
add_subdirectory(Network)
add_subdirectory(Physics)
add_subdirectory(InputManager)
add_subdirectory(Render)
add_subdirectory(GUI)
