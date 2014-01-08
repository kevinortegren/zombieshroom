# Find the Bullet library

find_path(BULLET_INCLUDE_DIRS
    Bullet/btBulletCollisionCommon.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(BULLETCOLLISION_LIBRARY_DEBUG
    NAMES BulletCollision_vs2010_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
find_library(BULLETDYNAMICS_LIBRARY_DEBUG
    NAMES BulletDynamics_vs2010_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
find_library(LINEARMATH_LIBRARY_DEBUG
    NAMES LinearMath_vs2010_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
	
find_library(BULLETCOLLISION_LIBRARY
    NAMES BulletCollision_vs2010
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")
find_library(BULLETDYNAMICS_LIBRARY
    NAMES BulletDynamics_vs2010
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")
find_library(LINEARMATH_LIBRARY
    NAMES LinearMath_vs2010
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")
	
find_library(BULLETCOLLISION_LIBRARY_DEBUG_64
    NAMES BulletCollision_vs2010_x64_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x64/Debug")
find_library(BULLETDYNAMICS_LIBRARY_DEBUG_64
    NAMES BulletDynamics_vs2010_x64_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x64/Debug")
find_library(LINEARMATH_LIBRARY_DEBUG_64
    NAMES LinearMath_vs2010_x64_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x64/Debug")