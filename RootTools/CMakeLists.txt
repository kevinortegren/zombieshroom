# CMake settings
cmake_minimum_required(VERSION 2.8)

# Add subdirectories for tools
if (COMPILE_LEVEL_EDITOR)
	add_subdirectory(LevelEditor)
endif()