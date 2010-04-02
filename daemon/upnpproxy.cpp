/*
    This file is part of the Cagibi daemon, part of the KDE project.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "upnpproxy.h"

// program
#include "upnpproxydbusadaptor.h"
#include "ssdpwatcher.h"
#include "rootdevice.h"
#include "device.h"


namespace Cagibi
{

UPnPProxy::UPnPProxy( QObject* parent )
  : QObject( parent ),
    mSsdpWatcher( new SSDPWatcher(this) )
{
    new UPnPProxyDBusAdaptor( this );

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    sessionBus.registerService( QString::fromLatin1("org.kde.Cagibi") );
    sessionBus.registerObject( QString::fromLatin1("/"), this );

    connect( mSsdpWatcher, SIGNAL(deviceDiscovered( Cagibi::RootDevice* )),
             SLOT(onDeviceDiscovered( Cagibi::RootDevice* )) );
    connect( mSsdpWatcher, SIGNAL(deviceRemoved( Cagibi::RootDevice* )),
             SLOT(onDeviceRemoved( Cagibi::RootDevice* )) );

    mSsdpWatcher->discover();
}

DeviceTypeMap UPnPProxy::allDevices() const
{
    return DeviceTypeMap();
}

Device UPnPProxy::deviceDetails( const QString& udn ) const
{
    return Device();
}

void UPnPProxy::onDeviceDiscovered( RootDevice* rootDevice )
{
    DeviceTypeMap devices;

    const Device device = rootDevice->device();
    devices.insert( device.udn(), device.type() );

    emit devicesAdded( devices );
}

void UPnPProxy::onDeviceRemoved( RootDevice* rootDevice )
{
    DeviceTypeMap devices;

    const Device device = rootDevice->device();
    devices.insert( device.udn(), device.type() );

    emit devicesRemoved( devices );
}

UPnPProxy::~UPnPProxy()
{
}

}
