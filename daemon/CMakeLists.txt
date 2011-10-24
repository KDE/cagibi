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

if(WIN32)
    target_link_libraries( cagibid ${KDEWIN_LIBRARIES})
endif(WIN32)


install( TARGETS cagibid  RUNTIME DESTINATION bin )

configure_file( org.kde.Cagibi.service.cmake ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Cagibi.service )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Cagibi.service DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/services )

if (NOT WIN32)
    configure_file( cagibi.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/cagibi.pc )
    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/cagibi.pc
             DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig )
endif (NOT WIN32)
