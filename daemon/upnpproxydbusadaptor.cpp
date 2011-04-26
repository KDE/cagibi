/*
    This file is part of the Cagibi daemon.

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

#include "upnpproxydbusadaptor.h"

// daemon
#include "dbuscodec.h"
// Qt
#include <QtDBus/QDBusMetaType>


namespace Cagibi
{

UPnPProxyDBusAdaptor::UPnPProxyDBusAdaptor( UPnPProxy* parent )
  : QDBusAbstractAdaptor( parent )
{
    connect( parent, SIGNAL(devicesAdded(DeviceTypeMap)),
             SIGNAL(devicesAdded(DeviceTypeMap)) );
    connect( parent, SIGNAL(devicesRemoved(DeviceTypeMap)),
             SIGNAL(devicesRemoved(DeviceTypeMap)) );

    // TODO: best place to do this?
    qDBusRegisterMetaType<DeviceTypeMap>();
    qDBusRegisterMetaType<Cagibi::Device>();
}

DeviceTypeMap UPnPProxyDBusAdaptor::allDevices() const
{
    return parent()->allDevices();
}

DeviceTypeMap UPnPProxyDBusAdaptor::devicesByParent( const QString& udn ) const
{
    return parent()->devicesByParent( udn );
}

DeviceTypeMap UPnPProxyDBusAdaptor::devicesByType( const QString& type ) const
{
    return parent()->devicesByType( type );
}

Device UPnPProxyDBusAdaptor::deviceDetails( const QString& udn ) const
{
    return parent()->deviceDetails( udn );
}

UPnPProxyDBusAdaptor::~UPnPProxyDBusAdaptor()
{
}

}
