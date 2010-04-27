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

#include "rootdevice.h"
// #include "rootdevice.moc"

// server
#include "devicedescriptionxmlhandler.h"
// #include "service.h"
// Qt
#include <QtNetwork/QNetworkReply>


namespace Cagibi
{

static bool parseDeviceDescription( RootDevice* device, const QByteArray& data, QString* error )
{
    bool result = true;

    QXmlInputSource input;
    input.setData( data );

    QXmlSimpleReader reader;
    DeviceDescriptionXMLHandler descriptionXMLHandler( device );
    reader.setContentHandler( &descriptionXMLHandler );

    result = reader.parse( &input, false );

    if( ! result )
        *error = QObject::tr( "Error parsing XML of device description." );

    return result;
}


void RootDevice::timerEvent( QTimerEvent* event )
{
    Q_UNUSED( event );

    emit cacheTimedOut( this );
}

#if 0
void RootDevice::startServiceDescriptionDownload( const Service& service )
{
    const QUrl location = mBaseUrl + service.descriptionUrl();
qDebug() << "Downloading service description from " << location;

    mError = QString();

//     KIO::Job* job = KIO::storedGet( location, KIO::NoReload, KIO::Overwrite | KIO::HideProgressInfo );
//     connect( job, SIGNAL(result( KJob* )), SLOT(onServiceDescriptionDownloadDone( KJob* )) );
//     mServiceDownloadJob[job] = service;
}
#endif
void RootDevice::onDeviceDescriptionDownloadReply( QNetworkReply* reply )
{
    bool success;

    if( reply->error() != QNetworkReply::NoError )
    {
        mError = QObject::tr( "Failed to download from \"%1\": %2").arg( mLocation.toString(), reply->error() );
qDebug() << mError;
        success = false;
    }
    else
    {
        const QByteArray deviceDescriptionData = reply->readAll();
qDebug()<< QString::fromAscii(deviceDescriptionData);
        success = parseDeviceDescription( this, deviceDescriptionData, &mError );
    }
    reply->deleteLater();

    emit deviceDescriptionDownloadDone( this, success );
}

#if 0
void RootDevice::onServiceDescriptionDownloadDone( KJob* job )
{
    bool success;

    Service service = mServiceDownloadJob[job];
    mServiceDownloadJob.remove( job );

    const QUrl location = mBaseUrl + service.descriptionUrl();

    if( job->error() )
    {
        mError = QObject::tr( "Failed to download service description from \"%1\": %2").arg(location.toString(), job->errorString() );
qDebug() << mError;
        success = false;
    }
    else
    {
        KIO::StoredTransferJob* storedTransferJob = static_cast<KIO::StoredTransferJob*>( job );
qDebug()<< QString::fromAscii(storedTransferJob->data());
        success = true;//parseDescription( p, storedTransferJob->data(), &mError );
    }
    emit serviceDescriptionDownloadDone( service, success );
}
#endif

#if 0
void RootDevice::onSoapReplyReceived( const QByteArray& reply, const QVariant& data )
{
    const bool isError = reply.isEmpty();
    if( isError )
    {
        mError = mSoapAgent->lastError();
qDebug() << "Error: " << mError;
    }
    else
    {
        if( reply.contains("Connected") )
        {
            Service service = data.value<Service>();
            service.setReady();
            mDevice.addService( service );

qDebug() << "Service added: " << service.type();
        }
    }
}
#endif

RootDevice::~RootDevice()
{
}

}
