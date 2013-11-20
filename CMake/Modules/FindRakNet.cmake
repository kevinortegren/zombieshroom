# Find the RakNet library

find_path(RAKNET_INCLUDE_DIRS
    RakNet/RakPeerInterface.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(RAKNET_LIBRARY
    NAMES RakNet_VS2008_DLL_Debug_Win32
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")