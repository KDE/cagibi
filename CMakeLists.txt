project( Cagibi )

cmake_minimum_required(VERSION 2.4.5 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${Cagibi_SOURCE_DIR}/cmake/modules")

find_package( Qt4 REQUIRED COMPONENTS QtCore QtXml QtDBus QtNetwork )
include(${QT_USE_FILE})

find_package(Automoc4 REQUIRED)

add_subdirectory( daemon )
