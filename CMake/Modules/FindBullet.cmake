# Find the Bullet library

find_path(BULLET_INCLUDE_DIRS
    Bullet/btBulletCollisionCommon.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")

find_library(BULLETCOLLISION_LIBRARY
    NAMES BulletCollision_vs2010_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")
find_library(BULLETDYNAMICS_LIBRARY
    NAMES BulletDynamics_vs2010_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")
find_library(LINEARMATH_LIBRARY
    NAMES LinearMath_vs2010_debug
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib")