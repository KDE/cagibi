/*
    This file is part of the KUPnP library, part of the KDE project.

    Copyright 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ssdpwatcher.h"

// lib
#include "rootdevice.h"
// KDE
#include <KUrl>
// Qt
#include <QtNetwork/QUdpSocket>
#include <QtCore/QStringList>
// C
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef Q_WS_WIN
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#endif

#include <KDebug>


namespace UPnP
{

static const int SSDPPortNumber = 1900;
static const char SSDPBroadCastAddress[] = "239.255.255.250";

// copied from KTorrent UPnP, but is it needed?
static void joinUPnPMCastGroup( int fd )
{
    ip_mreq mreq;

    memset( &mreq, 0, sizeof(ip_mreq) );

    inet_aton( SSDPBroadCastAddress, &mreq.imr_multiaddr );
    mreq.imr_interface.s_addr = htonl( INADDR_ANY );

#ifndef Q_WS_WIN
    if( setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(ip_mreq)) < 0 )
#else
    if( setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char *)&mreq,sizeof(ip_mreq)) < 0 )
#endif
kDebug() << "Failed to join multicast group 239.255.255.250";
}

static void leaveUPnPMCastGroup( int fd )
{
    struct ip_mreq mreq;

    memset( &mreq, 0, sizeof(ip_mreq) );

    inet_aton( SSDPBroadCastAddress, &mreq.imr_multiaddr );
    mreq.imr_interface.s_addr = htonl( INADDR_ANY );

#ifndef Q_WS_WIN
    if( setsockopt(fd,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(ip_mreq)) < 0 )
#else
    if( setsockopt(fd,IPPROTO_IP,IP_DROP_MEMBERSHIP,(char *)&mreq,sizeof(ip_mreq)) < 0 )
#endif
kDebug() << "Failed to leave multicast group 239.255.255.250";
}


SSDPWatcher::SSDPWatcher()
  : mUdpSocket( new QUdpSocket(this) )
{
    connect( mUdpSocket, SIGNAL(readyRead()),
             SLOT(onUdpSocketReadyRead()) );
    connect( mUdpSocket, SIGNAL(error( QAbstractSocket::SocketError )),
             SLOT(onUdpSocketError( QAbstractSocket::SocketError )) );

    // try up to ten port numbers TODO: make configurable
    for( int i = 0; i < 10; ++i )
    {
        if( ! mUdpSocket->bind(SSDPPortNumber+i,QUdpSocket::ShareAddress) )
kDebug() << "Cannot bind to UDP port "<< SSDPPortNumber << ":" << mUdpSocket->errorString();
        else
            break;
    }

    // TODO: really needed with QUdpSocket::ShareAddress ?
    joinUPnPMCastGroup( mUdpSocket->socketDescriptor() );
}


void SSDPWatcher::discover()
{
kDebug() << "Trying to find UPnP devices on the local network";

    // send a HTTP M-SEARCH message to 239.255.255.250:1900
    const char mSearchMessage[] =
        "M-SEARCH * HTTP/1.1\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "ST:urn:schemas-upnp-org:device:upnp:rootdevice:1\r\n"
//         "ST:urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n"
//         "ST:urn:schemas-upnp-org:device:WANDevice:1\r\n"
        "MAN:\"ssdp:discover\"\r\n"
        "MX:3\r\n"
        "\r\n";
    const int mSearchMessageLength = sizeof(mSearchMessage) / sizeof(mSearchMessage[0]);

    mUdpSocket->writeDatagram( mSearchMessage, mSearchMessageLength, QHostAddress(SSDPBroadCastAddress), SSDPPortNumber );
}


RootDevice* SSDPWatcher::createDeviceFromResponse( const QByteArray& response )
{
    RootDevice* device = 0;

    const QStringList lines = QString::fromAscii( response ).split( "\r\n" );

    // first read first line and see if contains a HTTP 200 OK message or
    // "HTTP/1.1 200 OK"
    // "NOTIFY * HTTP/1.1"
    const QString firstLine = lines.first();
    if( firstLine.contains("HTTP") )
    {
        // it is either a 200 OK or a NOTIFY
        if( ! firstLine.contains("NOTIFY") && ! firstLine.contains("200 OK") )
            return 0;
    }
    else
        return 0;

    QString server;
    KUrl location;
    QString uuid;
    enum MessageType { SearchAnswer, Notification, UnknownMessage };
    enum DeviceState { Alive, ByeBye, OtherState };
    DeviceState deviceState = OtherState;
    MessageType messageType = UnknownMessage;

    // read all lines and try to find the server and location fields
    foreach( const QString& line, lines )
    {
        const int separatorIndex = line.indexOf( ':' );
        const QString key = line.left( separatorIndex ).toUpper();
        const QString value = line.mid( separatorIndex+1 ).trimmed();

        if( key == QLatin1String("LOCATION") )
        {
kDebug()<<"LOCATION:"<<value;
            location = value;
        }
        else if( key == QLatin1String("SERVER") )
        {
kDebug()<<"SERVER:"<<value;
            server = value;
        }
        else if( key == QLatin1String("ST") ) // search type
        {
kDebug()<<"ST:"<<value;
            messageType = SearchAnswer;
        }
        else if( key == QLatin1String("NT") ) // notification type
        {
kDebug()<<"NT:"<<value;
            messageType = Notification;
        }
        else if( key == QLatin1String("NTS") ) // notification type s?
        {
kDebug()<<"NTS:"<<value;
            if( value == QLatin1String("ssdp:alive") )
                deviceState = Alive;
            else if( value == QLatin1String("ssdp:byebye") )
                deviceState = ByeBye;
        }
// TODO:         else if( key == QLatin1String("CACHE-CONTROL") )
// TODO:         else if( key == QLatin1String("DATE") )
        else if( key == QLatin1String("USN") ) // unique service name
        {
kDebug()<<"USN:"<<value;
            const int startIndex = 5;
            const int endIndex = value.lastIndexOf( "::" );
            int length = endIndex - startIndex;
            if( length == 0 )
                length = -1;
            uuid = value.mid( startIndex, length );
        }
    }

    if( uuid.isEmpty() )
    {
kDebug()<<"No uuid found!";
    }
    else if( location.isEmpty() )
    {
kDebug()<<"No location found!";
    }
    else if( deviceState == OtherState && messageType == Notification )
    {
kDebug()<<"NTS neither alive nor byebye";
    }
    else if( mDevices.contains(uuid) )
    {
kDebug()<<"Already inserted:"<<uuid<<"!";
    }
#if 0
    else if( ! mBrowsedDeviceTypes.isEmpty() && ! mBrowsedDeviceTypes.contains(devicePrivate->type()) )
    {
kDebug()<<"Not interested in:"<<devicePrivate->type();
        devicePrivate->setInvalid();
    }
#endif
    else
    {
kDebug() << "Detected Device:" << server << "UUID" << uuid;
        // everything OK, make a new Device
        device = new RootDevice( server, location, uuid );
    }

    return device;
}


void SSDPWatcher::onDescriptionDownloadDone( RootDevice* device, bool success )
{
    mPendingDevices.remove( device );

    if( ! success )
        device->deleteLater();
    else if( mDevices.contains(device->uuid()) )
        device->deleteLater();
    else
    {
        mDevices.insert( device->uuid(), device );
kDebug()<< "Added:"<<device->name()<<device->uuid();
        emit deviceDiscovered( device );
    }
}

void SSDPWatcher::onUdpSocketReadyRead()
{
    const int pendingDatagramSize = mUdpSocket->pendingDatagramSize();

    QByteArray response( pendingDatagramSize, 0 );
    const int bytesRead = mUdpSocket->readDatagram( response.data(), pendingDatagramSize );
    if( bytesRead == -1 )
        // TODO: error handling
        return;
kDebug()<<QString::fromAscii(response);
    RootDevice* device = createDeviceFromResponse( response );
    if( device )
    {
        connect( device, SIGNAL(deviceDescriptionDownloadDone( RootDevice*, bool )),
                 SLOT(onDeviceDescriptionDownloadDone( RootDevice*, bool )) );

        device->startDeviceDescriptionDownload();
        mPendingDevices.insert( device );
    }
}


void SSDPWatcher::onUdpSocketError( QAbstractSocket::SocketError error )
{
    Q_UNUSED( error );
kDebug() << "SSDPWatcher Error : " << mUdpSocket->errorString();
}


SSDPWatcher::~SSDPWatcher()
{
    leaveUPnPMCastGroup( mUdpSocket->socketDescriptor() );

    qDeleteAll( mPendingDevices );
    qDeleteAll( mDevices );
}

}
