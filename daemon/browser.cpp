/*
    This file is part of the Cagibi library, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy 
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// lib
#include "ssdpwatcher.h"
#include "rootdevice.h"
#include "device.h"
#include "service.h"
// Qt
#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>

void operator<<( QTextStream& out, const Cagibi::Device& device )
{
    static int indent = 0;
    static int indentDiff = 2;
    QString space( indent, ' ' );

    const QList<Cagibi::Service> services = device.services();

    out << space << device.friendlyName() << " ("<<device.type()<<")"<<endl;

    foreach( const Cagibi::Service& service, services )
        out << space << "* " << service.controlUrl() << " ("<<service.type()<<")"<<endl;

    indent += indentDiff;
    const QList<Cagibi::Device> devices = device.devices();
    foreach( const Cagibi::Device& device, devices )
        out << device;
    indent -= indentDiff;
}

int main( int argc, char* argv[] )
{
//     KAboutData aboutData( "kupnpbrowser2", "KUpnpBrowser2", ki18n(""), "", ki18n("") );
//     KCmdLineArgs::init( argc, argv, &aboutData );
    QCoreApplication programCore( argc, argv );

    Cagibi::SSDPWatcher deviceBrowser;
    deviceBrowser.discover();
    // run loop at least once
    QTimer::singleShot( 5000, &programCore, SLOT(quit()) );
    const int result = programCore.exec();

    const QList<Cagibi::RootDevice*> devices = deviceBrowser.devices();
    QTextStream out( stdout );
    out << "Found devices: " << endl;
    foreach( const Cagibi::RootDevice* rootDevice, devices )
    {
        const Cagibi::Device device = rootDevice->device();
        out << device;
    }

    out << "That's all." << endl;

    return result;
}
