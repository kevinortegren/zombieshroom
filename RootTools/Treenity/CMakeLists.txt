# CMake settings
cmake_minimum_required(VERSION 2.8.11)

# Root project settings
set(PROJECT_NAME Treenity
project(${PROJECT_NAME})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
file(GLOB_RECURSE SOURCES Source/*.cpp)
file(GLOB_RECURSE HEADERS Include/*.h)
file(GLOB_RECURSE UI_FILES UI/*.ui)
file(GLOB_RECURSE QRC_FILES UI/*.qrc)
file(GLOB_RECURSE GEN_FILES GeneratedFiles/*.*)
file(GLOB_RECURSE RESOURCE_FILES *.rc)
set(LIBRARIES ${EXTERNAL_LIBRARIES} DynamicLoader ECS RootSystems)

foreach(it ${UI_FILES})
    get_filename_component(outfile ${it} NAME_WE)
    get_filename_component(infile ${it} ABSOLUTE)
    set(outfile ${CMAKE_CURRENT_SOURCE_DIR}/GeneratedFiles/ui_${outfile}.h)
    add_custom_command(OUTPUT ${outfile}
      COMMAND ${CMAKE_SOURCE_DIR}/External/Bin/x86/Debug/uic.exe
      ARGS -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile} VERBATIM)
   # list(APPEND ${outfiles} ${outfile})
endforeach()

set(MOC_OPTIONS -DUNICODE -DWIN32 -DWIN64 -DQT_DLL -DQT_CORE_LIB -DQT_GUI_LIB -DQT_WIDGETS_LIB "-I${CMAKE_CURRENT_SOURCE_DIR}/GeneratedFiles" "-I${CMAKE_CURRENT_SOURCE_DIR}" "-I${QT_INCLUDE_DIRS}" "-I${CMAKE_CURRENT_SOURCE_DIR}/GeneratedFiles" "-I$(QT_INCLUDE_DIRS)/QtCore" "-I$(QT_INCLUDE_DIRS)/QtGui" "-I$(QT_INCLUDE_DIRS)/QtWidgets")
foreach(it ${HEADERS})
    get_filename_component(outfile ${it} NAME_WE)
    get_filename_component(infile ${it} ABSOLUTE)
    set(outfile ${CMAKE_CURRENT_SOURCE_DIR}/GeneratedFiles/moc_${outfile}.cpp)
    add_custom_command(OUTPUT ${outfile}
      COMMAND ${CMAKE_SOURCE_DIR}/External/Bin/x86/Debug/moc.exe
      ARGS ${MOC_OPTIONS} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile} VERBATIM)
   # list(APPEND ${outfiles} ${outfile})
endforeach()

#message("${SOURCES}")
#message("${HEADERS}")

# Set search paths
include_directories(Include)

# Add the target

add_executable(${PROJECT_NAME} ${UI_FILES} ${GEN_FILES} ${QRC_FILES} ${HEADERS} ${SOURCES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
