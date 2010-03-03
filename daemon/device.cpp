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

#include "device.h"

// server
#include "soapagent.h"
#include "devicedescriptionxmlhandler.h"
// KDE
#include <KIO/Job>
#include <KIO/NetAccess>
#include <KLocale>

#include <KDebug>

namespace UPnP
{

static bool parseDescription( Device* device, const QByteArray& data, QString* error )
{
    bool result = true;

    QXmlInputSource input;
    input.setData( data );

    QXmlSimpleReader reader;
    DeviceDescriptionXMLHandler descriptionXMLHandler( device);
    reader.setContentHandler( &descriptionXMLHandler );

    result = reader.parse( &input, false );

    if( ! result )
        *error = i18n( "Error parsing XML of device description." );

    return result;
}


Device::Device( const QString& name, const KUrl& location, const QString& uuid )
  : mName( name ),
    mLocation( location ),
    mUuid( uuid ),
    mSoapAgent( new SoapAgent(location,this) )
{
    connect( mSoapAgent, SIGNAL(replyReceived( const QByteArray&, const QVariant& )),
             SLOT(onSoapReplyReceived( const QByteArray&, const QVariant& )) );

}

void Device::addIcon( const Icon& icon )
{
kDebug()<<icon.url()<<icon.width()<<"x"<<icon.height();
    mIcons.append( icon );
}

void Device::addService( const Service& service )
{
    const QString& type = service.type();
    const QString actionId =  QString::fromLatin1( "GetStatusInfo" );
    const QString& url = service.controlUrl();
kDebug()<<type<<url;
    mSoapAgent->sendCommand( type, actionId, url, QVariant::fromValue<Service>(service) );
}


void Device::startDescriptionDownload()
{
kDebug() << "Downloading description from " << mLocation.prettyUrl();

    mError = QString();

    KIO::Job* job = KIO::storedGet( mLocation, KIO::NoReload, KIO::Overwrite | KIO::HideProgressInfo );
    connect( job, SIGNAL(result( KJob* )), SLOT(onDescriptionDownloadDone( KJob* )) );
}

void Device::onDescriptionDownloadDone( KJob* job )
{
    if( job->error() )
    {
        mError = i18n( "Failed to download from \"%1\": %2", mLocation.prettyUrl(), job->errorString() );
kDebug() << mError;
        return;
    }

    KIO::StoredTransferJob* storedTransferJob = static_cast<KIO::StoredTransferJob*>( job );
kDebug()<< QString::fromAscii(storedTransferJob->data());
    const bool success = parseDescription( this, storedTransferJob->data(), &mError );

    emit descriptionDownloadDone( this, success );
}


void Device::onSoapReplyReceived( const QByteArray& reply, const QVariant& data )
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
            mServices.append( service );

kDebug() << "Service added: " << service.type();
        }
    }
}


Device::~Device() {}

}
