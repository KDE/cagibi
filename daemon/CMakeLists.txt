include_directories(
)

set( kupnpserver_LIB_SRCS
  devicedescriptionxmlhandler.cpp
#   soapagent.cpp
  icon_p.cpp
  icon.cpp
  service_p.cpp
  service.cpp
  device_p.cpp
  device.cpp
  rootdevice_p.cpp
  rootdevice.cpp
  ssdpwatcher.cpp
)

kde4_add_library( kupnpserver  STATIC ${kupnpserver_LIB_SRCS} )

set( kupnpbrowser2_SRCS
  main.cpp
)

kde4_add_executable( kupnpbrowser2  ${kupnpbrowser2_SRCS} )

target_link_libraries( kupnpbrowser2
  kupnpserver
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTCORE_LIBRARY}
)

install( TARGETS kupnpbrowser2  ${INSTALL_TARGETS_DEFAULT_ARGS} )
