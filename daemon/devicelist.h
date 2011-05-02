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

#ifndef DEVICELIST_H
#define DEVICELIST_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMetaType>

class QTimer;

typedef QHash<QString,QString> DeviceTypeMap;
Q_DECLARE_METATYPE( DeviceTypeMap )


namespace Cagibi
{
class SSDPWatcher;
class RootDevice;
class Device;


class DeviceList : public QObject
{
    Q_OBJECT

  public:
    explicit DeviceList( int searchTimeout, int inactivityTimeout );
    virtual ~DeviceList();

  public:
    DeviceTypeMap allDevices() const;
    DeviceTypeMap devicesByParent( const QString& udn ) const;
    DeviceTypeMap devicesByType( const QString& type ) const;
    Device deviceDetails( const QString& udn ) const;

  Q_SIGNALS:
    void devicesAdded( const DeviceTypeMap& devices );
    void devicesRemoved( const DeviceTypeMap& devices );

    void gotInactiv();

  private Q_SLOTS:
    void onInitialSearchCompleted();
    void onDeviceDiscovered( Cagibi::RootDevice* rootDevice );
    void onDeviceRemoved( Cagibi::RootDevice* rootDevice );

  private:
    bool shutsDownOnNoActivity() const;

  private:
    SSDPWatcher* mSsdpWatcher;
    QTimer* mShutDownTimer;

    int mShutDownTimeout;
};

inline bool DeviceList::shutsDownOnNoActivity() const { return mShutDownTimeout != 0; }

}

#endif
