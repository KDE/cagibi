/*
    This file is part of the Cagibi library, part of the KDE project.

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
// Qt
#include <QtNetwork/QUdpSocket>
#include <QtCore/QUrl>
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

#include <QtCore/QDebug>


namespace Cagibi
{

#define SSDP_BROADCAST_ADDRESS "239.255.255.250"
#define SSDP_PORT_NUMBER 1900
#define SSDP_PORT "1900"

static const int SSDPPortNumber = SSDP_PORT_NUMBER;
static const char SSDPBroadCastAddress[] = SSDP_BROADCAST_ADDRESS;

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
qDebug() << "Failed to join multicast group " SSDP_BROADCAST_ADDRESS;
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
qDebug() << "Failed to leave multicast group " SSDP_BROADCAST_ADDRESS;
}


SSDPWatcher::SSDPWatcher( QObject* parent )
  : QObject( parent ),
    mUdpSocket( new QUdpSocket(this) )
{
    connect( mUdpSocket, SIGNAL(readyRead()),
             SLOT(onUdpSocketReadyRead()) );
    connect( mUdpSocket, SIGNAL(error( QAbstractSocket::SocketError )),
             SLOT(onUdpSocketError( QAbstractSocket::SocketError )) );

    // try up to ten port numbers TODO: make configurable
    for( int i = 0; i < 10; ++i )
    {
        if( ! mUdpSocket->bind(SSDPPortNumber+i,QUdpSocket::ShareAddress) )
qDebug() << "Cannot bind to UDP port "<< SSDPPortNumber << ":" << mUdpSocket->errorString();
        else
            break;
    }

    // TODO: really needed with QUdpSocket::ShareAddress ?
    joinUPnPMCastGroup( mUdpSocket->socketDescriptor() );
}


void SSDPWatcher::discover()
{
qDebug() << "Trying to find UPnP devices on the local network";

    // send a HTTP M-SEARCH message to 239.255.255.250:1900
    const char mSearchMessage[] =
        "M-SEARCH * HTTP/1.1\r\n"
        "HOST: "SSDP_BROADCAST_ADDRESS":"SSDP_PORT"\r\n"
        "ST:urn:schemas-upnp-org:device:upnp:rootdevice:1\r\n"
//         "ST: ssdp:all\r\n"
        "MAN:\"ssdp:discover\"\r\n"
        "MX:3\r\n" // max number of seconds to wait for response
        "\r\n";
    const int mSearchMessageLength = sizeof(mSearchMessage) / sizeof(mSearchMessage[0]);

    mUdpSocket->writeDatagram( mSearchMessage, mSearchMessageLength, QHostAddress(SSDPBroadCastAddress), SSDPPortNumber );
}


void SSDPWatcher::handleMessage( const QByteArray& message )
{
    const QStringList lines = QString::fromAscii( message ).split( "\r\n" );

    // first read first line and see if contains a HTTP 200 OK message or
    // "HTTP/1.1 200 OK"
    // "NOTIFY * HTTP/1.1"
    const QString firstLine = lines.first();
    if( ! firstLine.contains("HTTP")
        || (! firstLine.contains( "NOTIFY" )
            && ! firstLine.contains( "200 OK" )) )
        return;

    QString server;
    QUrl location;
    QString uuid;
    enum MessageType { SearchAnswer, Notification, UnknownMessage };
    enum DeviceState { Alive, ByeBye, OtherState };
    DeviceState deviceState = OtherState;
    MessageType messageType = UnknownMessage;
    int maxAge = 0; // in seconds

    // read all lines and try to find the server and location fields
    foreach( const QString& line, lines )
    {
        const int separatorIndex = line.indexOf( ':' );
        const QString key = line.left( separatorIndex ).toUpper();
        const QString value = line.mid( separatorIndex+1 ).trimmed();

        if( key == QLatin1String("LOCATION") )
        {
qDebug()<<"LOCATION:"<<value;
            location = value;
        }
        else if( key == QLatin1String("SERVER") )
        {
qDebug()<<"SERVER:"<<value;
            server = value;
        }
        else if( key == QLatin1String("ST") ) // search type
        {
qDebug()<<"ST:"<<value;
            messageType = SearchAnswer;
        }
        else if( key == QLatin1String("NT") ) // notification type
        {
qDebug()<<"NT:"<<value;
            messageType = Notification;
        }
        else if( key == QLatin1String("NTS") ) // notification type s(tatus)?
        {
qDebug()<<"NTS:"<<value;
            if( value == QLatin1String("ssdp:alive") )
                deviceState = Alive;
            else if( value == QLatin1String("ssdp:byebye") )
                deviceState = ByeBye;
        }
        else if( key == QLatin1String("CACHE-CONTROL") )
        {
            const int separatorIndex = line.indexOf( '=' );
            const QString key = line.left( separatorIndex ).trimmed().toUpper();
            const QString value = line.mid( separatorIndex+1 ).trimmed();
            if( key == QLatin1String("MAX-AGE") )
                maxAge = value.toInt();
        }
// TODO:         else if( key == QLatin1String("DATE") )
        else if( key == QLatin1String("USN") ) // unique service name
        {
qDebug()<<"USN:"<<value;
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
qDebug()<<"No uuid found!";
    }
    else if( location.isEmpty() )
    {
qDebug()<<"No location found!";
    }
    else if( deviceState == OtherState && messageType == Notification )
    {
qDebug()<<"NTS neither alive nor byebye";
    }
    else if( mDevices.contains(uuid) )
    {
qDebug()<<"Already inserted:"<<uuid<<"!";
    }
#if 0
    else if( ! mBrowsedDeviceTypes.isEmpty() && ! mBrowsedDeviceTypes.contains(devicePrivate->type()) )
    {
qDebug()<<"Not interested in:"<<devicePrivate->type();
        devicePrivate->setInvalid();
    }
#endif
    else
    {
qDebug() << "Detected Device:" << server << "UUID" << uuid;
        // everything OK, make a new Device
        RootDevice* device = new RootDevice( server, location, uuid );
        connect( device, SIGNAL(deviceDescriptionDownloadDone( RootDevice*, bool )),
                 SLOT(onDeviceDescriptionDownloadDone( RootDevice*, bool )) );

        mPendingDevices.insert( device );
        device->startDeviceDescriptionDownload();
    }
}


void SSDPWatcher::onDeviceDescriptionDownloadDone( RootDevice* device, bool success )
{
    mPendingDevices.remove( device );

    if( ! success )
        device->deleteLater();
    else if( mDevices.contains(device->uuid()) )
        device->deleteLater();
    else
    {
        mDevices.insert( device->uuid(), device );
qDebug()<< "Added:"<<device->name()<<device->uuid();
        emit deviceDiscovered( device );
    }
}

void SSDPWatcher::onUdpSocketReadyRead()
{
    const int pendingDatagramSize = mUdpSocket->pendingDatagramSize();

    QByteArray message( pendingDatagramSize, 0 );
    const int bytesRead = mUdpSocket->readDatagram( message.data(), pendingDatagramSize );
    if( bytesRead == -1 )
        // TODO: error handling
        return;
qDebug()<<QString::fromAscii(message);

    handleMessage( message );
}


void SSDPWatcher::onUdpSocketError( QAbstractSocket::SocketError error )
{
    Q_UNUSED( error );
qDebug() << "SSDPWatcher Error : " << mUdpSocket->errorString();
}


SSDPWatcher::~SSDPWatcher()
{
    leaveUPnPMCastGroup( mUdpSocket->socketDescriptor() );

    qDeleteAll( mPendingDevices );
    qDeleteAll( mDevices );
}

}
