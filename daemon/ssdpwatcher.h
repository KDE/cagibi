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

#ifndef SSDPWATCHER_H
#define SSDPWATCHER_H

// Qt
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QSet>
#include <QtCore/QObject>

class QUdpSocket;


namespace Cagibi
{
class RootDevice;

/// Simple Service Discovery Protocol
class SSDPWatcher : public QObject
{
  Q_OBJECT

  public:
    explicit SSDPWatcher( QObject* parent = 0 );

    virtual ~SSDPWatcher();

  public:
    int devicesCount() const;
    QList<RootDevice*> devices() const;

  public Q_SLOTS:
    void discover();

  Q_SIGNALS:
    void deviceDiscovered( Cagibi::RootDevice* device );
    void deviceRemoved( Cagibi::RootDevice* device );

  protected:
    void handleMessage( const QByteArray& message );

  protected Q_SLOTS:
    void onUdpSocketReadyRead();
    void onUdpSocketError( QAbstractSocket::SocketError error );
    void onDeviceDescriptionDownloadDone( RootDevice* device, bool success );
    void onCacheTimedOut( RootDevice* device );

  protected:
    QHash<QString,RootDevice*> mDevices;
    QSet<RootDevice*> mPendingDevices;

    QUdpSocket* mUdpSocket;
};


inline int SSDPWatcher::devicesCount() const { return mDevices.count(); }
inline QList<RootDevice*> SSDPWatcher::devices() const { return mDevices.values(); }

}

#endif
