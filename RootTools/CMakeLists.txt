# CMake settings
cmake_minimum_required(VERSION 2.8)

# Add subdirectories for tools
if (NOT COMPILE_LEVEL_EDITOR)
	if(NOT COMPILE_RELEASE_BUILD)
		add_subdirectory(RootServer)
		add_subdirectory(UnitTesting)
		add_subdirectory(AbilityEditor)
		add_subdirectory(ParticleEditor)
		add_subdirectory(Treenity)
	endif()
else()
	add_subdirectory(LevelEditor)
endif()