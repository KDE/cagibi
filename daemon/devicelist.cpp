/*
    This file is part of the Cagibi daemon.

    Copyright 2010-2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "devicelist.h"

// program
#include "ssdpwatcher.h"
#include "rootdevice.h"
#include "device.h"
// Qt
#include <QtCore/QTimer>


namespace Cagibi
{

static void fillMap( DeviceTypeMap& map, const Device& device )
{
    map.insert( device.udn(), device.type() );
    foreach( const Cagibi::Device& subDevice, device.devices() )
        fillMap( map, subDevice );
}

static void fillMapByType( DeviceTypeMap& map, const Device& device,
                           const QString& type )
{
    const QString deviceType = device.type();

    if( deviceType == type )
        map.insert( device.udn(), deviceType );

    foreach( const Cagibi::Device& subDevice, device.devices() )
        fillMapByType( map, subDevice, type );
}

static const Device* find( const Device& device, const QString& udn )
{
    const Device* result = 0;

    if( device.udn() == udn )
        result = &device;
    else
    {
        foreach( const Cagibi::Device& subDevice, device.devices() )
        {
            result = find( subDevice, udn );
            if( result )
                break;
        }
    }

    return result;
}


DeviceList::DeviceList( int searchTimeout, int inactivityTimeout )
  : QObject(),
    mSsdpWatcher( new SSDPWatcher(this) ),
    mShutDownTimeout( inactivityTimeout )
{
    // setup timer to shutdown on no UPnP activity
    mShutDownTimer = new QTimer( this );
    mShutDownTimer->setInterval( mShutDownTimeout * 1000 ); // in msec
    mShutDownTimer->setSingleShot( true );
    connect( mShutDownTimer, SIGNAL(timeout()),
             this, SIGNAL(gotInactiv()) );

    // install listener to UPnP changes
    connect( mSsdpWatcher, SIGNAL(deviceDiscovered( Cagibi::RootDevice* )),
             SLOT(onDeviceDiscovered( Cagibi::RootDevice* )) );
    connect( mSsdpWatcher, SIGNAL(deviceRemoved( Cagibi::RootDevice* )),
             SLOT(onDeviceRemoved( Cagibi::RootDevice* )) );
    connect( mSsdpWatcher, SIGNAL(initialSearchCompleted()),
             SLOT(onInitialSearchCompleted()) );

    mSsdpWatcher->startDiscover( searchTimeout );
}

DeviceTypeMap DeviceList::allDevices() const
{
    DeviceTypeMap result;

    const QList<RootDevice*> rootDevices = mSsdpWatcher->devices();

    foreach( RootDevice* rootDevice, rootDevices )
    {
        const Device device = rootDevice->device();
        fillMap( result, device );
    }

    // being used, so reset the shutdown timer if active
    if( mShutDownTimer->isActive() )
        mShutDownTimer->start();

    return result;
}

DeviceTypeMap DeviceList::devicesByParent( const QString& udn ) const
{
    DeviceTypeMap result;

    const QList<RootDevice*> rootDevices = mSsdpWatcher->devices();

    foreach( RootDevice* rootDevice, rootDevices )
    {
        const Device device = rootDevice->device();

        if( udn.isEmpty() )
            result.insert( device.udn(), device.type() );
        else
        {
            const Device* match = find( device, udn );
            if( match )
            {
                foreach( const Cagibi::Device& subDevice, device.devices() )
                    result.insert( subDevice.udn(), subDevice.type() );
                break;
            }
        }
    }

    // being used, so reset the shutdown timer if active
    if( mShutDownTimer->isActive() )
        mShutDownTimer->start();

    return result;
}

DeviceTypeMap DeviceList::devicesByType( const QString& type ) const
{
    DeviceTypeMap result;

    const QList<RootDevice*> rootDevices = mSsdpWatcher->devices();

    foreach( RootDevice* rootDevice, rootDevices )
    {
        const Device device = rootDevice->device();
        fillMapByType( result, device, type );
    }

    return result;
}

Device DeviceList::deviceDetails( const QString& udn ) const
{
    Device result;

    const QList<RootDevice*> rootDevices = mSsdpWatcher->devices();

    foreach( RootDevice* rootDevice, rootDevices )
    {
        const Device device = rootDevice->device();

        const Device* match = find( device, udn );
        if( match )
        {
            result = *match;
            break;
        }
    }

    return result;
}

void DeviceList::onInitialSearchCompleted()
{
    if( shutsDownOnNoActivity() && mSsdpWatcher->devicesCount() == 0 )
        mShutDownTimer->start();
}

void DeviceList::onDeviceDiscovered( RootDevice* rootDevice )
{
    DeviceTypeMap devices;

    const Device device = rootDevice->device();
    fillMap( devices, device );

    mShutDownTimer->stop();

    emit devicesAdded( devices );
}

void DeviceList::onDeviceRemoved( RootDevice* rootDevice )
{
    DeviceTypeMap devices;

    const Device device = rootDevice->device();
    fillMap( devices, device );

    if( shutsDownOnNoActivity() && mSsdpWatcher->devicesCount() == 0 )
        mShutDownTimer->start();

    emit devicesRemoved( devices );
}

DeviceList::~DeviceList()
{
    // simulate that all devices are removed
    const QList<RootDevice*> rootDevices = mSsdpWatcher->devices();
    foreach( RootDevice* rootDevice, rootDevices )
        onDeviceRemoved( rootDevice );
}

}
