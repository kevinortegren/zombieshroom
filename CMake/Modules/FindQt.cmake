find_path(QT_INCLUDE_DIRS
    QtGui/qguiapplication.h
    HINTS "${CMAKE_SOURCE_DIR}/External/Include/Qt")


find_library(QTGUI_LIBRARY
    NAMES Qt5Guid
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
find_library(QTCORE_LIBRARY
    NAMES Qt5Cored
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
find_library(QTWIDGETS_LIBRARY
    NAMES Qt5Widgetsd
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")