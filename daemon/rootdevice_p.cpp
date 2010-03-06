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

#include "rootdevice_p.h"

// server
#include "devicedescriptionxmlhandler.h"
#include "service.h"
// KDE
#include <KIO/Job>
#include <KIO/NetAccess>

#include <QtCore/QDebug>

namespace UPnP
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


void RootDevicePrivate::startDeviceDescriptionDownload()
{
qDebug() << "Downloading description from " << mLocation;

    mError = QString();

    KIO::Job* job = KIO::storedGet( mLocation, KIO::NoReload, KIO::Overwrite | KIO::HideProgressInfo );
    p->connect( job, SIGNAL(result( KJob* )), SLOT(onDeviceDescriptionDownloadDone( KJob* )) );
}

void RootDevicePrivate::startServiceDescriptionDownload( const Service& service )
{
    const QUrl location = mBaseUrl + service.descriptionUrl();
qDebug() << "Downloading service description from " << location;

    mError = QString();

    KIO::Job* job = KIO::storedGet( location, KIO::NoReload, KIO::Overwrite | KIO::HideProgressInfo );
    p->connect( job, SIGNAL(result( KJob* )), SLOT(onServiceDescriptionDownloadDone( KJob* )) );
    mServiceDownloadJob[job] = service;
}

void RootDevicePrivate::onDeviceDescriptionDownloadDone( KJob* job )
{
    bool success;

    if( job->error() )
    {
        mError = QObject::tr( "Failed to download from \"%1\": %2").arg( mLocation.toString(), job->errorString() );
qDebug() << mError;
        success = false;
    }
    else
    {
        KIO::StoredTransferJob* storedTransferJob = static_cast<KIO::StoredTransferJob*>( job );
qDebug()<< QString::fromAscii(storedTransferJob->data());
        success = parseDeviceDescription( p, storedTransferJob->data(), &mError );
    }
    emit p->deviceDescriptionDownloadDone( p, success );
}


void RootDevicePrivate::onServiceDescriptionDownloadDone( KJob* job )
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
    emit p->serviceDescriptionDownloadDone( service, success );
}


void RootDevicePrivate::onSoapReplyReceived( const QByteArray& reply, const QVariant& data )
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


RootDevicePrivate::~RootDevicePrivate() {}

}
