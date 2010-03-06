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
// KDE
#include <KIO/Job>
#include <KIO/NetAccess>
#include <KLocale>

#include <KDebug>

namespace UPnP
{

static bool parseDescription( RootDevice* device, const QByteArray& data, QString* error )
{
    bool result = true;

    QXmlInputSource input;
    input.setData( data );

    QXmlSimpleReader reader;
    DeviceDescriptionXMLHandler descriptionXMLHandler( device );
    reader.setContentHandler( &descriptionXMLHandler );

    result = reader.parse( &input, false );

    if( ! result )
        *error = i18n( "Error parsing XML of device description." );

    return result;
}


void RootDevicePrivate::startDeviceDescriptionDownload()
{
kDebug() << "Downloading description from " << mLocation.prettyUrl();

    mError = QString();

    KIO::Job* job = KIO::storedGet( mLocation, KIO::NoReload, KIO::Overwrite | KIO::HideProgressInfo );
    p->connect( job, SIGNAL(result( KJob* )), SLOT(onDeviceDescriptionDownloadDone( KJob* )) );
}

void RootDevicePrivate::onDeviceDescriptionDownloadDone( KJob* job )
{
    bool success;

    if( job->error() )
    {
        mError = i18n( "Failed to download from \"%1\": %2", mLocation.prettyUrl(), job->errorString() );
kDebug() << mError;
        success = false;
    }
    else
    {
        KIO::StoredTransferJob* storedTransferJob = static_cast<KIO::StoredTransferJob*>( job );
kDebug()<< QString::fromAscii(storedTransferJob->data());
        success = parseDescription( p, storedTransferJob->data(), &mError );
    }
    emit p->deviceDescriptionDownloadDone( p, success );
}


void RootDevicePrivate::onSoapReplyReceived( const QByteArray& reply, const QVariant& data )
{
    const bool isError = reply.isEmpty();
    if( isError )
    {
        mError = mSoapAgent->lastError();
kDebug() << "Error: " << mError;
    }
    else
    {
        if( reply.contains("Connected") )
        {
            Service service = data.value<Service>();
            service.setReady();
            mDevice.addService( service );

kDebug() << "Service added: " << service.type();
        }
    }
}


RootDevicePrivate::~RootDevicePrivate() {}

}
