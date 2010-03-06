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

#include "soapagent.h"

// KDE
#include <KIO/Job>
// Qt
#include <QtCore/QList>


#include <QtCore/QDebug>

namespace UPnP
{
static const int defaultSoapPort = 80;

struct SoapArgument
{
    QString key;
    QString value;
};

static QString createSoapCommand( const QString& action, const QString& service )
{
    QString command = QString::fromLatin1(
        "<?xml version=\"1.0\"?>"
        "<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                           "SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
            "<SOAP-ENV:Body>"
                "<m:%1 xmlns:m=\"%2\"/>"
            "</SOAP-ENV:Body>"
        "</SOAP-ENV:Envelope>" ).arg( action, service );

    return command;
}
#if 0
static QString createSoapCommand( const QString& action, const QString& service,
                                  const QList<SoapArgument>& arguments )
{
    QString command = QString::fromLatin1(
        "<?xml version=\"1.0\"?>"
        "<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                           "SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
            "<SOAP-ENV:Body>"
                "<m:%1 xmlns:m=\"%2\">" ).arg( action, service );

    foreach( const SoapArgument& argument, arguments )
        command += '<' + argument.key + '>' + argument.value + "</" + argument.key + '>';

    command += QString::fromLatin1(
                "</m:%1>"
            "</SOAP-ENV:Body>"
        "</SOAP-ENV:Envelope>").arg( action );

    return command;
}
#endif


SoapAgent::SoapAgent( const QUrl& location, QObject* parent )
  : QObject( parent ),
    mLocation( location )
{
    // ensure port
    if( mLocation.port() <= 0 )
        mLocation.setPort( defaultSoapPort );
}


void SoapAgent::sendCommand( const QString& type, const QString& actionId, const QString& url,
                             const QVariant& data )
{
    const QString command = createSoapCommand( actionId, type );
    const QString action = QString::fromLatin1( "%1#%2" ).arg( type, actionId );

    KUrl address;
    address.setProtocol( QString::fromLatin1("http") );
    address.setHost( mLocation.host() );
    address.setPort( mLocation.port() );
    address.setPath( url );

    KIO::TransferJob* postCommandJob = KIO::http_post( address, command.toAscii(), KIO::HideProgressInfo );

    postCommandJob->addMetaData( "content-type",     QString::fromLatin1("text/xml"));
    postCommandJob->addMetaData( "UserAgent",        QString::fromLatin1("KUPnP"));
    postCommandJob->addMetaData( "customHTTPHeader", QString::fromLatin1("SOAPAction: ") + action );

    mWriteBufferPerJob[postCommandJob] = QByteArray();
    mReadBufferPerJob[postCommandJob]  = QByteArray();
    mPendingDataPerJob[postCommandJob]  = data;

    connect( postCommandJob, SIGNAL(data( KIO::Job*, const QByteArray& )),
             SLOT(receiveData( KIO::Job*, const QByteArray& )) );
    connect( postCommandJob, SIGNAL(dataReq( KIO::Job*, QByteArray& )),
             SLOT(sendData( KIO::Job*, QByteArray& )) );
    connect( postCommandJob, SIGNAL(result( KJob* )),
             SLOT(onPostCommandJobFinished( KJob* )) );
}

void SoapAgent::sendData( KIO::Job* job, QByteArray& data )
{
    QByteArray& soapData = mWriteBufferPerJob[job];
    data.append( soapData );
    soapData.clear();
}

void SoapAgent::receiveData( KIO::Job* job, const QByteArray& data )
{
    mReadBufferPerJob[job].append( data );
}

void SoapAgent::onPostCommandJobFinished( KJob* job )
{
    if( job->error() )
    {
        mError = job->errorString();
qDebug() << "Error: " << mError;
    }
    else
    {
        const QByteArray soapReply = mReadBufferPerJob[job];
        const QVariant data = mPendingDataPerJob[job];

qDebug() << "OK!";

        emit replyReceived( soapReply, data );
    }

    mReadBufferPerJob.remove( job );
    mWriteBufferPerJob.remove( job );
    mPendingDataPerJob.remove( job );
}


SoapAgent::~SoapAgent() {}

}
