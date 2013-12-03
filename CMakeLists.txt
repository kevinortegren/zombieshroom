# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(ROOT_PROJECT_NAME ZombieShroom)
project(${ROOT_PROJECT_NAME})

# Check if we want to compile just the level editor.
option(COMPILE_LEVEL_EDITOR "Should we compile level editor?" OFF)

# Find external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

if (NOT COMPILE_LEVEL_EDITOR)
	find_package(OpenGL REQUIRED)
	find_package(SDL2 REQUIRED)
	find_package(GTest REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(RakNet REQUIRED)
	find_package(Bullet REQUIRED)
	find_package(Assimp REQUIRED)
	find_package(GLM REQUIRED)
	find_package(YAML REQUIRED)
	find_package(Rocket REQUIRED)

	set(EXTERNAL_INCLUDE_DIRS 
		${SDL2_INCLUDE_DIRS}
		${GTEST_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${RAKNET_INCLUDE_DIRS}
		${BULLET_INCLUDE_DIRS}
		${ASSIMP_INCLUDE_DIRS}
		${GLM_INCLUDE_DIRS}
		${YAML_INCLUDE_DIRS}
		${ROCKET_INCLUDE_DIRS}
	)
    
	set(EXTERNAL_LIBRARIES
		${SDL2_LIBRARY}
		${GTEST_LIBRARY}
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARY}
		${RAKNET_LIBRARY}
		${ASSIMP_LIBRARY}
		${BULLETCOLLISION_LIBRARY}
		${BULLETDYNAMICS_LIBRARY}
		${LINEARMATH_LIBRARY}
		${YAML_LIBRARY_DEBUG}
		${ROCKET_CONTROLS_LIBRARY}
		${ROCKET_CORE_LIBRARY}
		${ROCKET_DEBUGGER_LIBRARY}
	)
else()
	find_package(OpenGL REQUIRED)
	find_package(SDL2 REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(GLM REQUIRED)
	find_package(YAML REQUIRED)

	set(EXTERNAL_INCLUDE_DIRS 
		${SDL2_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${GLM_INCLUDE_DIRS}
		${YAML_INCLUDE_DIRS}
	)
    
	set(EXTERNAL_LIBRARIES
		${SDL2_LIBRARY_64}
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARY_64}
		${YAML_LIBRARY_DEBUG_64}
	)
	
	add_definitions(-DCOMPILE_LEVEL_EDITOR)
endif()

message("${EXTERNAL_LIBRARIES}")

# Set global compiler options
add_definitions(-D_VARIADIC_MAX=10)
add_definitions(-DUNICODE -D_UNICODE)

# Add sub projects and dependencies.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${EXTERNAL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})

if (NOT COMPILE_LEVEL_EDITOR)
	add_subdirectory(RootEngine)
	add_subdirectory(RootForce)
	add_subdirectory(RootTools)
	add_subdirectory(Utility)
	add_subdirectory(Assets)
else()
	add_subdirectory(RootEngine)
	add_subdirectory(RootTools)
	add_subdirectory(Utility)
	add_subdirectory(Assets)
endif()

# Copy the runtime dependencies to the output directory when dependency target is built
# TODO: This is hardcoded to the Debug folder. We cannot use a Release build...
if (NOT COMPILE_LEVEL_EDITOR)
add_custom_target(_Dependencies ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/External/Bin/x86 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
)
else()
add_custom_target(_Dependencies ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/External/Bin/x64 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
)
endif()
 
