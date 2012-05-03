/*
    This file is part of the Cagibi daemon.

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

// program
#include "device.h"
// #include "service.h"
// #include "soapagent.h"
// Qt
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QUrl>
#include <QtCore/QTimerEvent>
#include <QtCore/QObject>

#include <QtCore/QDebug>

class QNetworkReply;
class QUrl;


namespace Cagibi
{

class RootDevice : public QObject
{
  Q_OBJECT

  public:
    RootDevice( const QString& name, const QUrl& location, const QString& uuid );

    virtual ~RootDevice();

  public:
    const QString& name() const;
    const QString& uuid() const;
    const QUrl& location() const;

    const Device& device() const;

    const QString& lastError() const;

  public:
    void startDeviceDescriptionDownload();
//     void startServiceDescriptionDownload( const Service& service );
    void setBaseUrl( const QString& baseUrl );
    void setDevice( const Device& device );
    // in seconds
    void resetCacheTimeOut( int timeout );

  Q_SIGNALS:
    void deviceDescriptionDownloadDone( RootDevice* device, bool success );
//     void serviceDescriptionDownloadDone( const Service& service, bool success );

    void cacheTimedOut( RootDevice* device );

  protected: // QObject API
     virtual void timerEvent( QTimerEvent* event );

  protected Q_SLOTS:
    void onDeviceDescriptionDownloadReply( QNetworkReply* reply );
//     void onServiceDescriptionDownloadDone( KJob* job );
//     void onSoapReplyReceived( const QByteArray& reply, const QVariant& data );

  protected:
    QString mName;
    QUrl mLocation;
    QString mUuid;
    QString mBaseUrl;

    Device mDevice;

//     SoapAgent* mSoapAgent;
    QNetworkAccessManager* mNetworkAccessManager;

//     QHash<KJob*,Service> mServiceDownloadJob;

    QString mError;
    int mTimerId;
};


inline RootDevice::RootDevice( const QString& name, const QUrl& location, const QString& uuid )
  : QObject(),
    mName( name ),
    mLocation( location ),
    mUuid( uuid ),
//     mSoapAgent( new SoapAgent(location,this) ),
    mNetworkAccessManager( new QNetworkAccessManager(this) ),
    mTimerId( 0 )
{
    connect( mNetworkAccessManager, SIGNAL(finished(QNetworkReply*) ),
             SLOT(onDeviceDescriptionDownloadReply(QNetworkReply*)) );

//     connect( mSoapAgent, SIGNAL(replyReceived( const QByteArray&, const QVariant& )),
//              SLOT(onSoapReplyReceived( const QByteArray&, const QVariant& )) );
}

inline const QString& RootDevice::name()      const { return mName; }
inline const QString& RootDevice::uuid()      const { return mUuid; }
inline const QUrl& RootDevice::location()     const { return mLocation; }
inline const Device& RootDevice::device()     const { return mDevice; }
inline const QString& RootDevice::lastError() const { return mError; }

inline void RootDevice::setBaseUrl( const QString& baseUrl ) { mBaseUrl = baseUrl; }
inline void RootDevice::setDevice( const Device& device )    { mDevice = device; }

inline void RootDevice::resetCacheTimeOut( int timeout )
{
    if( mTimerId != 0 )
        killTimer( mTimerId );

    mTimerId = startTimer( timeout * 1000 );
}

inline void RootDevice::startDeviceDescriptionDownload()
{
qDebug() << "Downloading description from " << mLocation;

    mError = QString();

    mNetworkAccessManager->get( QNetworkRequest(mLocation) );
}

}

#endif
