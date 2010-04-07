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

#include "device_p.h"

// lib
#include "service_p.h"

#include <QtCore/QDebug>

namespace Cagibi
{

const char* const DevicePrivate::keys[] =
{
    "deviceType",
    "friendlyName",
    "manufacturer",
    "modelDescription",
    "modelName",
    "modelNumber",
    "serialNumber",
    "UDN",
    "presentationURL"
};

void DevicePrivate::addIcon( const Icon& icon )
{
qDebug()<<icon.url()<<icon.width()<<"x"<<icon.height();
    mIcons.append( icon );
}

void DevicePrivate::addService( const Service& service )
{
    const QString& type = service.type();
    const QString actionId =  QString::fromLatin1( "GetStatusInfo" );
    const QString& url = service.controlUrl();
qDebug()<<type<<url;
// TODO
#if 0
    mSoapAgent->sendCommand( type, actionId, url, QVariant::fromValue<Service>(service) );
#endif
//     service.setReady();
    service.d.data()->setDevicePrivate( this );
    mServices.append( service );
}

void DevicePrivate::addDevice( const Device& device )
{
qDebug()<<device.friendlyName();
    device.d.data()->setParentDevicePrivate( this );
    mDevices.append( device );
}

}
