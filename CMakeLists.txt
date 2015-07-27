cmake_minimum_required(VERSION 2.8.9)
project( Cagibi )

# CMP0005: keep escaping behaviour for definitions added via add_definitions()
cmake_policy(SET CMP0005 OLD)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_MODULE_PATH "${Cagibi_SOURCE_DIR}/cmake/modules")

find_package( Qt4 REQUIRED COMPONENTS QtCore QtXml QtDBus QtNetwork )
include(${QT_USE_FILE})

find_package(Automoc4 REQUIRED)

if(WIN32)
    find_package(KDEWin REQUIRED)
    include_directories(${KDEWIN_INCLUDES})
endif(WIN32)


set(CAGIBI_VERSION_MAJOR "0")
set(CAGIBI_VERSION_MINOR "2")
set(CAGIBI_VERSION_PATCH "0")
set(CAGIBI_VERSION "${CAGIBI_VERSION_MAJOR}.${CAGIBI_VERSION_MINOR}.${CAGIBI_VERSION_PATCH}")
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)


if (NOT DEFINED SYSCONF_INSTALL_DIR)
  if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
    set(SYSCONF_INSTALL_DIR  "/etc") # conform to LFSH
  else("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
    set(SYSCONF_INSTALL_DIR  "${CMAKE_INSTALL_PREFIX}/etc")
  endif("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
else (NOT DEFINED SYSCONF_INSTALL_DIR)
  set(SYSCONF_INSTALL_DIR "${SYSCONF_INSTALL_DIR}" CACHE STRING "The sysconfig install dir")
endif (NOT DEFINED SYSCONF_INSTALL_DIR)


if (NOT DEFINED CAGIBI_DAEMON_SYSTEMUSER)
  set(CAGIBI_DAEMON_SYSTEMUSER "cagibi" CACHE STRING "System user which will be used to run the daemon")
endif()

if (NOT DEFINED CAGIBI_DAEMON_SHUTDOWN_SECS)
  set(CAGIBI_DAEMON_SHUTDOWN_SECS "0" CACHE STRING "Default time without services and activity to trigger shutdown (0 = no shutdown)")
endif()

if (NOT DEFINED CAGIBI_DAEMON_SEARCH_TIMEOUT_SECS)
  set(CAGIBI_DAEMON_SEARCH_TIMEOUT_SECS "3" CACHE STRING "Default search timeout when querying for devices on a new network")
endif()

add_definitions(
  -DCAGIBI_DAEMON_SYSTEMUSER=${CAGIBI_DAEMON_SYSTEMUSER}
  -DCAGIBI_DAEMON_SHUTDOWN_SECS=${CAGIBI_DAEMON_SHUTDOWN_SECS}
  -DCAGIBI_DAEMON_SEARCH_TIMEOUT_SECS=${CAGIBI_DAEMON_SEARCH_TIMEOUT_SECS}
  -DSYSCONF_INSTALL_DIR=\\"${SYSCONF_INSTALL_DIR}\\"
)

add_subdirectory( daemon )
