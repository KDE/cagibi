/*
    This file is part of the Cagibi library, part of the KDE project.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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


QDBusArgument& operator<<( QDBusArgument& argument, const Cagibi::Device& device )
{
    const Cagibi::DevicePrivate* devicePrivate = device.d.constData();

    argument.beginStructure();

    argument << devicePrivate->type()
             << devicePrivate->friendlyName()
             << devicePrivate->manufacturerName()
//     const QString& manufacturerUrl() const;
             << devicePrivate->modelDescription()
             << devicePrivate->modelName()
             << devicePrivate->modelNumber()
             << devicePrivate->serialNumber()
             << devicePrivate->udn()
//     const QString upc() const;
             << devicePrivate->presentationUrl();

    argument << ( devicePrivate->hasParentDevice() ?
        devicePrivate->parentDevice().udn() :
        QString() );

    Cagibi::RootDevice* rootDevice = devicePrivate->rootDevice();
    QUrl location = rootDevice ? rootDevice->location() : QUrl();
    argument << location.host()
             << location.port();

//     const QList<Icon>& icons() const;
//     const QList<Service>& services() const;
//     const QList<Device>& devices() const;

    argument.endStructure();

    return argument;
}

const QDBusArgument& operator>>( const QDBusArgument& argument,
                                 Cagibi::Device& device )
{
    Cagibi::DevicePrivate* devicePrivate = device.d.data();

    argument.beginStructure();

    argument >> devicePrivate->mType
             >> devicePrivate->mFriendlyName
             >> devicePrivate->mManufacturerName
//     const QString& manufacturerUrl() const;
             >> devicePrivate->mModelDescription
             >> devicePrivate->mModelName
             >> devicePrivate->mModelNumber
             >> devicePrivate->mSerialNumber
             >> devicePrivate->mUdn
//     const QString upc() const;
             >> devicePrivate->mPresentationUrl;


    QString parentDeviceUdn;
    argument >> parentDeviceUdn;
//     << ( devicePrivate->hasParentDevice() ?
//         devicePrivate->parentDevice().udn() :
//         QString() );

//     const QList<Icon>& icons() const;
//     const QList<Service>& services() const;
//     const QList<Device>& devices() const;

    argument.endStructure();

    return argument;
}
