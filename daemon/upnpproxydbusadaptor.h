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

#ifndef UPNPPROXYDBUSADAPTOR_H
#define UPNPPROXYDBUSADAPTOR_H

// program
#include "upnpproxy.h"
#include "dbuscodec.h"
// Qt
#include <QtDBus/QtDBus>
#include <QtCore/QObject>


namespace Cagibi
{

class UPnPProxyDBusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.Cagibi")

  public:
    explicit UPnPProxyDBusAdaptor( UPnPProxy* parent );

    virtual ~UPnPProxyDBusAdaptor();

  public:
    UPnPProxy* parent() const;

  public Q_SLOTS:
    DeviceTypeMap allDevices() const;
    Cagibi::Device deviceDetails( const QString& udn ) const;

  Q_SIGNALS:
    void devicesAdded( const DeviceTypeMap& devices );
    void devicesRemoved( const DeviceTypeMap& devices );
};


inline UPnPProxy* UPnPProxyDBusAdaptor::parent() const { return static_cast<UPnPProxy*>( QObject::parent() ); }

}

#endif
