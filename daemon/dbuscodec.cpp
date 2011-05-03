/*
    This file is part of the Cagibi daemon.

    Copyright 2010-2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "dbuscodec.h"

// network
#include "device_p.h"
#include "rootdevice.h"
// Qt
#include <QtDBus/QDBusArgument>
#include <QtCore/QUrl>

static const QString type = QLatin1String( "deviceType" );
static const QString friendlyName = QLatin1String( "friendlyName" );
static const QString manufacturerName = QLatin1String( "manufacturer" );
static const QString modelDescription = QLatin1String( "modelDescription" );
static const QString modelName = QLatin1String( "modelName" );
static const QString modelNumber = QLatin1String( "modelNumber" );
static const QString serialNumber = QLatin1String( "serialNumber" );
static const QString udn = QLatin1String( "UDN" );
static const QString presentationUrl = QLatin1String( "presentationURL" );
static const QString ipAddress = QLatin1String( "ipAddress" );
static const QString portNumber = QLatin1String( "ipPortNumber" );
static const QString parentDeviceUdn = QLatin1String( "parentDeviceUDN" );


QDBusArgument& operator<<( QDBusArgument& argument, const Cagibi::Device& device )
{
    const Cagibi::DevicePrivate* const devicePrivate = device.d.constData();
    const Cagibi::RootDevice* const rootDevice = devicePrivate->rootDevice();
    const bool isValid = ( rootDevice != 0 );

    argument.beginMap( QVariant::String, QVariant::String );
    if( isValid ) // is not valid e.g. if tested for signature
    {
        argument.beginMapEntry();
            argument << type << devicePrivate->type();
        argument.endMapEntry();
        argument.beginMapEntry();
            argument << friendlyName << devicePrivate->friendlyName();
        argument.endMapEntry();
        argument.beginMapEntry();
            argument << manufacturerName << devicePrivate->manufacturerName();
        argument.endMapEntry();
//     const QString& manufacturerUrl() const;
        argument.beginMapEntry();
            argument << modelDescription << devicePrivate->modelDescription();
        argument.endMapEntry();
        argument.beginMapEntry();
            argument << modelName << devicePrivate->modelName();
        argument.endMapEntry();
        argument.beginMapEntry();
            argument << modelNumber << devicePrivate->modelNumber();
        argument.endMapEntry();
        argument.beginMapEntry();
            argument << serialNumber << devicePrivate->serialNumber();
        argument.endMapEntry();
        argument.beginMapEntry();
            argument << udn << devicePrivate->udn();
        argument.endMapEntry();
//     const QString upc() const;
        argument.beginMapEntry();
            argument << presentationUrl << devicePrivate->presentationUrl();
        argument.endMapEntry();

        const QUrl& location = rootDevice->location();
        argument.beginMapEntry();
            argument << ipAddress << location.host();
        argument.endMapEntry();
        argument.beginMapEntry();
            argument << portNumber << QString::number(location.port());
        argument.endMapEntry();

        if( devicePrivate->hasParentDevice() )
        {
        argument.beginMapEntry();
            argument << parentDeviceUdn << devicePrivate->parentDevice().udn();
        argument.endMapEntry();
        }
//     const QList<Icon>& icons() const;
//     const QList<Service>& services() const;
//     const QList<Device>& devices() const;
    }
    argument.endMap();

    return argument;
}

// not used currently, so noop, but symbol needed for QtDBus integration
const QDBusArgument& operator>>( const QDBusArgument& argument,
                                 Cagibi::Device& device )
{
    Q_UNUSED( device )

    return argument;
}
