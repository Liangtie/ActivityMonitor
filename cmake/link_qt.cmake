cmake_minimum_required(VERSION 3.14)

# Link Qt
set(QT_LIBRARIES_NAMES
  Core
  Gui
  Widgets
  OpenGL
  Svg
  Concurrent
  LinguistTools
  Network
  Quick
  Sql
)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_LIBRARIES_NAMES})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_LIBRARIES_NAMES})
include_directories(%{Qt:QT_INSTALL_PREFIX})

list(APPEND QT_LIBRARIES
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::OpenGL
  Qt${QT_VERSION_MAJOR}::Svg
  Qt${QT_VERSION_MAJOR}::Concurrent
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Sql
)