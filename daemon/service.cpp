/*
    This file is part of the KUPnP library, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "service.h"
#include "service_p.h"


namespace UPnP
{
bool Service::isKey( const QString& key ) { return ServicePrivate::isKey(key); }


Service::Service()
  : d( new ServicePrivate() )
{
}

Service::Service( ServicePrivate* _d )
  : d( _d )
{
}

Service::Service( const Service& other )
  : d( other.d )
{
}


QString Service::type() const { return d->type(); }
bool Service::isReady() const { return d->isReady(); }
QString Service::id() const { return d->id(); }
QString Service::descriptionUrl() const { return d->descriptionUrl(); }
QString Service::controlUrl() const { return d->controlUrl(); }
QString Service::eventSubUrl() const { return d->eventSubUrl(); }

void Service::setProperty( const QString& key, const QString& value )
{
    d->setProperty( key, value );
};

void Service::setReady() { d->setReady(); }

Service& Service::operator=( const Service& other )
{
    d = other.d;
    return *this;
}

bool Service::operator==( const Service& other ) const
{
    return ( d == other.d );
}

Service::~Service()
{
}

}
