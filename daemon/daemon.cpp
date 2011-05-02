/*
    This file is part of the Cagibi daemon.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// program
#include "devicelistdbusadaptor.h"
#include "devicelist.h"
#include "daemon.h"
// Qt
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>


namespace Cagibi
{

static const int defaultShutDownTimeout = CAGIBI_DAEMON_SHUTDOWN_SECS;
static const int defaultSearchTimeout = CAGIBI_DAEMON_SEARCH_TIMEOUT_SECS;

Daemon::Daemon( int argc, char* argv[] )
  : QCoreApplication( argc, argv )
{
}


int Daemon::exec()
{
    // read settings
    QSettings::setPath( QSettings::NativeFormat, QSettings::SystemScope,
                        QLatin1String(SYSCONF_INSTALL_DIR) );
    QSettings settings( QSettings::SystemScope, QLatin1String("cagibid") );
    int shutDownTimeout =
        settings.value( QLatin1String("ShutDownTimeout"),
                        defaultShutDownTimeout ).toInt();
    const int searchTimeout =
        settings.value( QLatin1String("SearchTimeout"),
                        defaultSearchTimeout ).toInt();

    // publish service on D-Bus
    QDBusConnection dBusConnection = QDBusConnection::systemBus();
    dBusConnection.registerService( QLatin1String("org.kde.Cagibi") );

    DeviceList deviceList( searchTimeout, shutDownTimeout );
    connect( &deviceList, SIGNAL(gotInactiv()), SLOT(quit()) );
    new DeviceListDBusAdaptor( &deviceList );
    dBusConnection.registerObject( QLatin1String("/org/kde/Cagibi/DeviceList"), &deviceList );

    // do the mainloop
    const int result = QCoreApplication::exec();

    return result;
}

}
