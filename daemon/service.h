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

#ifndef SERVICE_H
#define SERVICE_H

// Qt
#include <QtCore/QMetaType>
#include <QtCore/QString>


namespace UPnP
{

class Service
{
  public:
    static const char* const keys[];
    enum { ServiceType=0, ControlUrl, EventSubUrl, SCPDUrl, ServiceId, KeyCount };

    static bool isKey( const QString& key );

  public:
    Service();
    Service( const Service& other );

  public:
    const QString& type() const;
    const QString& controlUrl() const;
    bool isReady() const;

  public:
    void setProperty( const QString& key, const QString& value );
    void setReady();
    void clear();

    Service& operator =( const Service& other );

  protected:
    QString mType;
    int mTypeVersion;
    QString mId;
    QString mDescriptionUrl;
    QString mControlUrl;
    QString mEventSubUrl;

    bool mReady;
};


inline Service::Service()
  : mReady( false )
{
}

inline const QString& Service::type()       const { return mType; }
inline const QString& Service::controlUrl() const { return mControlUrl; }
inline bool Service::isReady()              const { return mReady; }

inline void Service::setReady() { mReady = true; }


inline Service::Service( const Service& other )
{
    mType = other.mType;
    mControlUrl = other.mControlUrl;
    mEventSubUrl = other.mEventSubUrl;
    mId = other.mId;
    mDescriptionUrl = other.mDescriptionUrl;

    mReady = false;
}

inline void Service::setProperty( const QString& key, const QString& value )
{
    if( key == keys[ServiceType] )
        mType = value;
    else if( key == keys[ControlUrl] )
        mControlUrl = value;
    else if( key == keys[EventSubUrl] )
        mEventSubUrl = value;
    else if( key == keys[SCPDUrl] )
        mDescriptionUrl = value;
    else if( key == keys[ServiceId] )
        mId = value;
}

inline void Service::clear()
{
    mType.clear();
    mControlUrl.clear();
    mEventSubUrl.clear();
    mDescriptionUrl.clear();
    mId.clear();
}

inline Service & Service::operator =( const Service& other )
{
    mType = other.mType;
    mControlUrl = other.mControlUrl;
    mEventSubUrl = other.mEventSubUrl;
    mId = other.mId;
    mDescriptionUrl = other.mDescriptionUrl;

    return *this;
}

inline bool Service::isKey( const QString& key )
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

Q_DECLARE_METATYPE(UPnP::Service)

#endif
