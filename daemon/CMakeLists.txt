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
  browser.cpp
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


set( cagibid_SRCS
    upnpproxy.cpp
    upnpproxydbusadaptor.cpp
    dbuscodec.cpp
    main.cpp
)

# qt4_add_dbus_adaptor( cagibid_SRCS  org.kde.upnpproxy.xml upnpproxy.h UpnpProxy )

kde4_add_executable( cagibid  ${cagibid_SRCS} )

target_link_libraries( cagibid
    kupnpserver
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
)

install( TARGETS cagibid  ${INSTALL_TARGETS_DEFAULT_ARGS} )


configure_file( org.kde.cagibi.service.cmake ${CMAKE_CURRENT_BINARY_DIR}/org.kde.cagibi.service )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.cagibi.service DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/services )
