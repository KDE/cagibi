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

#ifndef DEVICE_H
#define DEVICE_H

// Qt
#include <QtCore/QExplicitlySharedDataPointer>

class QString;
template< class C > class QList;


namespace UPnP
{
class DevicePrivate;
class Icon;
class Service;


class Device
{
  friend class DevicePrivate;
  friend class ServicePrivate;

  public:
    static bool isKey( const QString& key );

  protected:
    explicit Device( DevicePrivate* _d );

  public:
    Device();
    Device( const Device& other );
    ~Device();

  public:
    Device& operator=( const Device& other );
    bool operator==( const Device& other ) const;

  public:
    QString type() const;
    QString friendlyName() const;
    QString manufacturerName() const;
//     QString manufacturerUrl() const;
    QString modelDescription() const;
    QString modelName() const;
    QString modelNumber() const;
    QString serialNumber() const;
    QString udn() const;
//     QString upc() const;
    QString presentationUrl() const;

    QList<Icon> icons() const;
    QList<Service> services() const;
    QList<Device> devices() const;

    bool hasParentDevice() const;
    Device parentDevice() const;

  public:
    void setProperty( const QString& key, const QString& value );

    void addDevice( const Device& device );
    void addService( const Service& service );
    void addIcon( const Icon& icon );

  protected:
    QExplicitlySharedDataPointer<DevicePrivate> d;
};

}

#endif
