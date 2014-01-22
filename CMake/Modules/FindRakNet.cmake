# Find the RakNet library

find_path(RAKNET_INCLUDE_DIRS
    RakNet/RakPeerInterface.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(RAKNET_LIBRARY_DEBUG
    NAMES RakNet_VS2008_DLL_Debug_Win32
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
find_library(RAKNET_LIBRARY
    NAMES RakNet_VS2008_DLL_Release_Win32
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")
	
find_library(RAKNET_LIBRARY
    NAMES RakNet_VS2008_DLL_Debug_x64
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x64/Debug")
find_library(RAKNET_LIBRARY
    NAMES RakNet_VS2008_DLL_Release_x64
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x64/Release")