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
find_library(QTPROPWIDGET_LIBRARY
    NAMES QtSolutions_PropertyBrowser-headd
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")
find_library(QTCOLORTRIANGLE_LIBRARY
    NAMES QtSolutions_ColorTriangle-2.6d
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Debug")

#RELEASE FINDS
find_library(QTGUI_LIBRARY_RELEASE
    NAMES Qt5Gui
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")
find_library(QTCORE_LIBRARY_RELEASE
    NAMES Qt5Core
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")
find_library(QTWIDGETS_LIBRARY_RELEASE
    NAMES Qt5Widgets
    HINTS "${CMAKE_SOURCE_DIR}/External/Lib/x86/Release")