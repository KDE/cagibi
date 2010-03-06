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
#include <QtCore/QExplicitlySharedDataPointer>

class QString;


namespace UPnP
{
class ServicePrivate;
class Device;


class Service
{
  friend class DevicePrivate;

  public:
    static bool isKey( const QString& key );

  protected:
    explicit Service( ServicePrivate* _d );

  public:
    Service();
    Service( const Service& other );
    ~Service();

  public:
    Service& operator=( const Service& other );
    bool operator==( const Service& other ) const;

  public:
    QString type() const;
    QString id() const;
    QString descriptionUrl() const;
    QString controlUrl() const;
    QString eventSubUrl() const;
    bool isReady() const;

    Device device() const;

  public:
    void setProperty( const QString& key, const QString& value );
    void setReady();

  protected:
    QExplicitlySharedDataPointer<ServicePrivate> d;
};

}

Q_DECLARE_METATYPE(UPnP::Service)

#endif
