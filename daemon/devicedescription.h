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

#ifndef DEVICEDESCRIPTION_H
#define DEVICEDESCRIPTION_H

// Qt
#include <QtCore/QString>


namespace UPnP
{

class DeviceDescription
{
  public:
    static const char* const keys[];
    enum { FriendlyName=0, Manufacturer, ModelDescription, ModelName, ModelNumber, SerialNumber, UDN, PresentationUrl, KeyCount };

    static bool isKey( const QString& key );

  public:
    const QString& friendlyName() const;

  public:
    void setProperty( const QString& key, const QString& value );

  protected:
    /// short user-friendly title
    QString mFriendlyName;
    QString mManufacturerName;
//     QString mManufacturerUrl;
    /// long user-friendly title
    QString mModelDescription;
    QString mModelName;
    QString mModelNumber;
    QString mSerialNumber;
    QString mUdn;
//     QString mUpc; Universal Product Code;
    QString mPresentationUrl;
};


inline const QString& DeviceDescription::friendlyName() const { return mFriendlyName; }

inline void DeviceDescription::setProperty( const QString& key, const QString& value )
{
    if( key == keys[FriendlyName] )
        mFriendlyName = value;
    else if( key == keys[Manufacturer] )
        mManufacturerName = value;
    else if( key == keys[ModelDescription] )
        mModelDescription = value;
    else if( key == keys[ModelName] )
        mModelName = value;
    else if( key == keys[ModelNumber] )
        mModelNumber == value;
    else if( key == keys[SerialNumber] )
        mSerialNumber == value;
    else if( key == keys[UDN] )
        mUdn == value;
    else if( key == keys[PresentationUrl] )
        mPresentationUrl == value;
}

inline bool DeviceDescription::isKey( const QString& key )
{
    bool result = false;

    for( int i=0; i<KeyCount; ++i )
        if( key == QLatin1String(keys[i]) )
        {
            result = true;
            break;
        }

    return result;
}

}

#endif
