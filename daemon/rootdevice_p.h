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

#ifndef ROOTDEVICE_P_H
#define ROOTDEVICE_P_H

// lib
#include "rootdevice.h"
#include "device.h"
#include "service.h"
// #include "soapagent.h"
// Qt
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QUrl>

class QNetworkReply;


namespace UPnP
{
class SoapAgent;


class RootDevicePrivate
{
  public:
    RootDevicePrivate( const QString& name, const QUrl& location, const QString& uuid, RootDevice* _p );

    ~RootDevicePrivate();

  public:
    const QString& name() const;
    const QString& uuid() const;
    const QUrl& location() const;
    const Device& device() const;
    const QString& lastError() const;

  public:
    void startDeviceDescriptionDownload();
    void startServiceDescriptionDownload( const Service& service );
    void setBaseUrl( const QString& baseUrl );
    void setDevice( const Device& device );

  public: // slots
    void onDeviceDescriptionDownloadReply( QNetworkReply* reply );
//     void onServiceDescriptionDownloadDone( KJob* job );
//     void onSoapReplyReceived( const QByteArray& reply, const QVariant& data );

  protected:
    RootDevice* p;

    QString mName;
    QUrl mLocation;
    QString mUuid;
    QString mBaseUrl;

    Device mDevice;

//     SoapAgent* mSoapAgent;
    QNetworkAccessManager* mNetworkAccessManager;

//     QHash<KJob*,Service> mServiceDownloadJob;

    QString mError;
};



inline RootDevicePrivate::RootDevicePrivate( const QString& name, const QUrl& location, const QString& uuid,
                                             RootDevice* _p )
  : p( _p ),
    mName( name ),
    mLocation( location ),
    mUuid( uuid ),
//     mSoapAgent( new SoapAgent(location,p) ),
    mNetworkAccessManager( new QNetworkAccessManager(p) )
{
    p->connect( mNetworkAccessManager, SIGNAL(finished( QNetworkReply*) ),
                SLOT(onDeviceDescriptionDownloadReply( QNetworkReply* )) );

//     p->connect( mSoapAgent, SIGNAL(replyReceived( const QByteArray&, const QVariant& )),
//                 SLOT(onSoapReplyReceived( const QByteArray&, const QVariant& )) );
}

inline const QString& RootDevicePrivate::name() const { return mName; }
inline const QString& RootDevicePrivate::uuid() const { return mUuid; }
inline const QUrl& RootDevicePrivate::location() const { return mLocation; }
inline const Device& RootDevicePrivate::device() const { return mDevice; }
inline const QString& RootDevicePrivate::lastError() const { return mError; }

inline void RootDevicePrivate::setBaseUrl( const QString& baseUrl ) { mBaseUrl = baseUrl; }
inline void RootDevicePrivate::setDevice( const Device& device ) { mDevice = device; }

}

#endif
