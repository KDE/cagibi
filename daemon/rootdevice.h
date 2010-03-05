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

#ifndef ROOTDEVICE_H
#define ROOTDEVICE_H

// Qt
#include <QtCore/QObject>

class KJob;
class KUrl;


namespace UPnP
{
class Device;
class SoapAgent;
class RootDevicePrivate;


class RootDevice : public QObject
{
  Q_OBJECT

  friend class RootDevicePrivate;

  public:
    RootDevice( const QString& name, const KUrl& location, const QString& uuid );

    virtual ~RootDevice();

  public:
    QString name() const;
    QString uuid() const;
    KUrl location() const;

    Device device() const;

    QString lastError() const;

  public:
    void startDescriptionDownload();
    void setBaseUrl( const QString& baseUrl );
    void setDevice( const Device& device );

  Q_SIGNALS:
    void descriptionDownloadDone( RootDevice* device, bool success );

  protected:
    Q_PRIVATE_SLOT( d, void onDescriptionDownloadDone( KJob* job ) )
    Q_PRIVATE_SLOT( d, void onSoapReplyReceived( const QByteArray& reply, const QVariant& data ) )

  protected:
    RootDevicePrivate* d;
};

}

#endif
