include_directories(
)

set( kupnpserver_LIB_SRCS
  devicedescriptionxmlhandler.cpp
  devicedescription.cpp
  soapagent.cpp
  icon.cpp
  service.cpp
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
  ${KDE4_KIO_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTCORE_LIBRARY}
)

install( TARGETS kupnpbrowser2  ${INSTALL_TARGETS_DEFAULT_ARGS} )
