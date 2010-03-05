/*
    This file is part of the KUPnP library, part of the KDE project.

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

#include "devicedescriptionxmlhandler.h"

// lib
#include "rootdevice.h"
#include "device.h"
// Qt
#include <QtXml/QXmlAttributes>

#include <KDebug>

namespace UPnP
{

DeviceDescriptionXMLHandler::DeviceDescriptionXMLHandler( RootDevice* device )
  : mRootDevice( device ),
    mCurrentDevice( device->device() )
{
}


bool DeviceDescriptionXMLHandler::startDocument()
{
    mStatusStack.push( TopLevel );
    return true;
}

bool DeviceDescriptionXMLHandler::endDocument()
{
    mStatusStack.pop();
    return true;
}

bool DeviceDescriptionXMLHandler::startElement( const QString& namespaceURI, const QString& localName,
                                                const QString& qName,
                                                const QXmlAttributes& attributes )
{
    Q_UNUSED( namespaceURI );
    Q_UNUSED( qName );
    Q_UNUSED( attributes );

    bool result = true;

    mCharacterData.clear();

    switch( mStatusStack.top() )
    {
    case TopLevel:
        if( localName == QLatin1String("root") )
            mStatusStack.push( RootElement );
        else
            result = false;
        break;
    case RootElement:
        if( localName == QLatin1String("device") )
            mStatusStack.push( DeviceElement );
        else if( localName == QLatin1String("URLBase") )
            mStatusStack.push( UrlBaseElement );
        else
            mStatusStack.push( UnknownElement );
        break;
    case DeviceElement:
        if( Device::isKey(localName) )
            mStatusStack.push( DataElement );
        else if( localName == QLatin1String("iconList") )
            mStatusStack.push( IconListElement );
        else if( localName == QLatin1String("serviceList") )
            mStatusStack.push( ServiceListElement );
        else if( localName == QLatin1String("deviceList") )
            mStatusStack.push( DeviceListElement );
        else
            mStatusStack.push( UnknownElement );
        break;
    case IconListElement:
        if( localName == QLatin1String("icon") )
            mStatusStack.push( IconElement );
        else
            mStatusStack.push( UnknownElement );
        break;
    case IconElement:
        if( Icon::isKey(localName) )
            mStatusStack.push( DataElement );
        else
            mStatusStack.push( UnknownElement );
        break;
    case ServiceListElement:
        if( localName == QLatin1String("service") )
            mStatusStack.push( ServiceElement );
        else
            mStatusStack.push( UnknownElement );
        break;
    case ServiceElement:
        if( Service::isKey(localName) )
            mStatusStack.push( DataElement );
        else
            mStatusStack.push( UnknownElement );
        break;
    case DeviceListElement:
        if( localName == QLatin1String("device") )
        {
            mStatusStack.push( DeviceElement );
            mParentDevice = mCurrentDevice;
            mCurrentDevice = Device();
        }
        else
            mStatusStack.push( UnknownElement );
        break;
    case UnknownElement:
            mStatusStack.push( UnknownElement );
        break;
    case UrlBaseElement:
    case DataElement:
        break;
    }

    return result;
}


bool DeviceDescriptionXMLHandler::endElement( const QString& namespaceURI, const QString& localName,
                                              const QString& qName )
{
    Q_UNUSED( namespaceURI );
    Q_UNUSED( qName );

    const Element oldStatus = mStatusStack.pop();
    switch( oldStatus )
    {
    case DataElement:
        if( mStatusStack.top() == DeviceElement )
            mCurrentDevice.setProperty( localName, mCharacterData );
        else if( mStatusStack.top() == IconElement )
            mCurrentIcon.setProperty( localName, mCharacterData );
        else if( mStatusStack.top() == ServiceElement )
            mCurrentService.setProperty( localName, mCharacterData );
        break;
    case IconElement:
        mCurrentDevice.addIcon( mCurrentIcon );
        mCurrentIcon = Icon();
        break;
    case ServiceElement:
        mCurrentDevice.addService( mCurrentService );
        mCurrentService = Service();
        break;
    case DeviceElement:
        if( mStatusStack.top() == RootElement )
            mRootDevice->setDevice( mCurrentDevice );
        else
        {
            mParentDevice.addDevice( mCurrentDevice );
            mCurrentDevice = mParentDevice;
            mParentDevice = mParentDevice.parentDevice();
        }
        break;
    case UrlBaseElement:
        mRootDevice->setBaseUrl( mCharacterData );
        break;
    default:
        break;
    }

    mCharacterData.clear();

    return true;
}


bool DeviceDescriptionXMLHandler::characters( const QString& characters )
{
    mCharacterData += characters;
    return true;
}

DeviceDescriptionXMLHandler::~DeviceDescriptionXMLHandler() {}

}
