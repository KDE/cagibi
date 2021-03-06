/*
    This file is part of the Cagibi daemon.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "device.h"
#include "device_p.h"


namespace Cagibi
{
bool Device::isKey( const QString& key ) { return DevicePrivate::isKey(key); }


Device::Device()
  : d( new DevicePrivate() )
{
}

Device::Device( DevicePrivate* _d )
  : d( _d )
{
}

Device::Device( const Device& other )
  : d( other.d )
{
}

const QString& Device::type() const { return d->type(); }
const QString& Device::friendlyName() const { return d->friendlyName(); }
const QString& Device::manufacturerName() const { return d->manufacturerName(); }
const QString& Device::modelDescription() const { return d->modelDescription(); }
const QString& Device::modelName() const { return d->modelName(); }
const QString& Device::modelNumber() const { return d->modelNumber(); }
const QString& Device::serialNumber() const { return d->serialNumber(); }
const QString& Device::udn() const { return d->udn(); }
const QString& Device::presentationUrl() const { return d->presentationUrl(); }

const QList<Icon>& Device::icons() const { return d->icons(); }
const QList<Service>& Device::services() const { return d->services(); }
const QList<Device>& Device::devices() const { return d->devices(); }

bool Device::hasParentDevice() const { return d->hasParentDevice(); }
Device Device::parentDevice() const { return d->parentDevice(); }
RootDevice* Device::rootDevice() const { return d->rootDevice(); }

void Device::setProperty( const QString& key, const QString& value )
{
    d->setProperty( key, value );
}
void Device::setRootDevice( RootDevice* rootDevice )
{
    d->setRootDevice( rootDevice );
}

void Device::addDevice( const Device& device )
{
    d->addDevice( device );
}
void Device::addService( const Service& service )
{
    d->addService( service );
}
void Device::addIcon( const Icon& icon )
{
    d->addIcon( icon );
}


Device& Device::operator=( const Device& other )
{
    d = other.d;
    return *this;
}

bool Device::operator==( const Device& other ) const
{
    return ( d == other.d );
}

Device::~Device()
{
}

}
