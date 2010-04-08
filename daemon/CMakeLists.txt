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

kde4_add_executable( cagibid  ${cagibid_SRCS} )

target_link_libraries( cagibid
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTCORE_LIBRARY}
)

install( TARGETS cagibid  ${INSTALL_TARGETS_DEFAULT_ARGS} )


configure_file( org.kde.cagibi.service.cmake ${CMAKE_CURRENT_BINARY_DIR}/org.kde.cagibi.service )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.cagibi.service DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/services )
