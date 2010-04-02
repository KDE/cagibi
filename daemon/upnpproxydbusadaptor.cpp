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
    setAutoRelaySignals( true );

    // TODO: best place to do this?
    qDBusRegisterMetaType< Cagibi::Device >();
    qDBusRegisterMetaType< QVector<Cagibi::Device> >();
}
#if 0
NetDevice UPnPProxyDBusAdaptor::deviceData( const QString& hostAddress )
{
    return parent()->deviceData( hostAddress );
}
NetService UPnPProxyDBusAdaptor::serviceData( const QString& hostAddress, const QString& serviceName, const QString& serviceType )
{
    return parent()->serviceData( hostAddress, serviceName, serviceType );
}
NetDeviceList UPnPProxyDBusAdaptor::deviceDataList()
{
    return parent()->deviceDataList();
}
NetServiceList UPnPProxyDBusAdaptor::serviceDataList( const QString& hostAddress )
{
    return parent()->serviceDataList( hostAddress );
}
#endif
UPnPProxyDBusAdaptor::~UPnPProxyDBusAdaptor()
{
}

}
