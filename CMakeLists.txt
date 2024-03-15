cmake_minimum_required(VERSION 3.14)

project(ECO LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Xml Widgets X11Extras REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Xml Widgets X11Extras REQUIRED)

add_executable(ECO
    main.cpp
    src/virt.cpp
    src/server.cpp
    src/startup.cpp
    src/settings.cpp
    src/power.cpp
    src/spice/spice.cpp
    src/spice/spice-view.cpp
    src/spice/spice-widget.cpp
    qml.qrc
)

target_compile_definitions(ECO
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(ECO
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::X11Extras X11 virt pthread udev gobject-2.0 glib-2.0 spice-client-glib-2.0)
target_include_directories(ECO PUBLIC
  /usr/include/gstreamer-1.0
  /usr/lib/x86_64-linux-gnu/glib-2.0/include
  /usr/include/glib-2.0
  /usr/lib64/glib-2.0/include
  /usr/include/spice-client-glib-2.0
  /usr/include/pixman-1
  /usr/include/spice-1)

# ecopen
add_executable(ecopen
    src/ecopen.cpp
)

target_compile_definitions(ecopen
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(ecopen
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Xml)
