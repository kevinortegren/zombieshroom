# Find the GLM library

find_path(GLM_INCLUDE_DIRS
    glm/glm.hpp
    HINTS "${CMAKE_SOURCE_DIR}/External/Include")