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

// lib
#include "icon.h"
#include "service.h"
#include "devicedescription.h"
// KDE
#include <KUrl>
// Qt
#include <QtCore/QObject>

class KJob;


namespace UPnP
{
class Service;
class SoapAgent;


class RootDevice : public QObject
{
  Q_OBJECT

  public:
    RootDevice( const QString& name, const KUrl& location, const QString& uuid );

    virtual ~RootDevice();

  public:
    const QString& name() const;
    const QString& uuid() const;
    const KUrl& location() const;
    const DeviceDescription& description() const;
    const QString& lastError() const;

  public:
    void startDescriptionDownload();
    void addService( const Service& service );
    void addIcon( const Icon& icon );

    DeviceDescription& description();

  Q_SIGNALS:
    void descriptionDownloadDone( RootDevice* device, bool success );

  protected Q_SLOTS:
    void onDescriptionDownloadDone( KJob* job );
    void onSoapReplyReceived( const QByteArray& reply, const QVariant& data );

  protected:
    QString mName;
    KUrl mLocation;
    QString mUuid;
    DeviceDescription mDescription;

    QList<Icon> mIcons;
    QList<Service> mServices;

    SoapAgent* mSoapAgent;

    QString mError;
};


inline const QString& RootDevice::name() const { return mName; }
inline const QString& RootDevice::uuid() const { return mUuid; }
inline const KUrl& RootDevice::location() const { return mLocation; }
inline const DeviceDescription& RootDevice::description() const { return mDescription; }
inline const QString& RootDevice::lastError() const { return mError; }

inline DeviceDescription& RootDevice::description() { return mDescription; }

}

#endif
