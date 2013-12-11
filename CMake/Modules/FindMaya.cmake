# Find Maya

find_path(MAYA_INCLUDE_DIRS
    maya/MViewportRenderer.h
    HINTS "${CMAKE_SOURCE_DIR}/RootTools/LevelEditor/External/Include")

find_library(MAYA_FOUNDATION_LIB
    NAMES Foundation
    HINTS "${CMAKE_SOURCE_DIR}/RootTools/LevelEditor/External/Lib")

find_library(MAYA_OPEN_MAYA_LIB
    NAMES OpenMaya
    HINTS "${CMAKE_SOURCE_DIR}/RootTools/LevelEditor/External/Lib")



set(MAYA_LIBRARIES ${MAYA_FOUNDATION_LIB} ${MAYA_OPEN_MAYA_LIB})