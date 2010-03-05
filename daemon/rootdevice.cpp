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

#include "rootdevice.h"
#include "rootdevice_p.h"
#include "rootdevice.moc"


namespace UPnP
{


RootDevice::RootDevice( const QString& name, const KUrl& location, const QString& uuid )
  : QObject(),
    d( new RootDevicePrivate(name,location,uuid,this) )
{
}

const QString& RootDevice::name() const { return d->name(); }
const QString& RootDevice::uuid() const { return d->uuid(); }
const KUrl& RootDevice::location() const { return d->location(); }
const Device& RootDevice::description() const { return d->description(); }
const QString& RootDevice::lastError() const { return d->lastError(); }

Device& RootDevice::description() { return d->description(); }

void RootDevice::startDescriptionDownload()
{
    d->startDescriptionDownload();
}


RootDevice::~RootDevice()
{
    delete d;
}

}
