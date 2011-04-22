include_directories(
)

set( cagibid_SRCS
    devicedescriptionxmlhandler.cpp
    icon_p.cpp
    icon.cpp
    service_p.cpp
    service.cpp
    device_p.cpp
    device.cpp
    rootdevice.cpp
    ssdpwatcher.cpp
    upnpproxy.cpp
    upnpproxydbusadaptor.cpp
    dbuscodec.cpp
    main.cpp
)

automoc4_add_executable( cagibid  ${cagibid_SRCS} )

target_link_libraries( cagibid
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTCORE_LIBRARY}
)

install( TARGETS cagibid  RUNTIME DESTINATION bin )

configure_file( org.kde.Cagibi.service.cmake ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Cagibi.service )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Cagibi.service DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/system-services )
install( FILES org.kde.Cagibi.conf DESTINATION ${SYSCONF_INSTALL_DIR}/dbus-1/system.d/ )

