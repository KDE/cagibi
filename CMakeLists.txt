project( Cagibi )

cmake_minimum_required(VERSION 2.4.5 FATAL_ERROR)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_MODULE_PATH "${Cagibi_SOURCE_DIR}/cmake/modules")

find_package( Qt4 REQUIRED COMPONENTS QtCore QtXml QtDBus QtNetwork )
include(${QT_USE_FILE})

find_package(Automoc4 REQUIRED)

set(CAGIBI_VERSION_MAJOR "0")
set(CAGIBI_VERSION_MINOR "2")
set(CAGIBI_VERSION_PATCH "0")
set(CAGIBI_VERSION "${CAGIBI_VERSION_MAJOR}.${CAGIBI_VERSION_MINOR}.${CAGIBI_VERSION_PATCH}")

set(SYSCONF_INSTALL_DIR  "${CMAKE_INSTALL_PREFIX}/etc")

add_subdirectory( daemon )
