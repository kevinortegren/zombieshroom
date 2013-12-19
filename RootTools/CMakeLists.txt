# CMake settings
cmake_minimum_required(VERSION 2.8)

# Add subdirectories for tools
if (NOT COMPILE_LEVEL_EDITOR)
	add_subdirectory(RootServer)
  add_subdirectory(UnitTesting)
else()
	add_subdirectory(LevelEditor)
endif()