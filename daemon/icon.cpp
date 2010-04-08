/*
    This file is part of the Cagibi library, part of the KDE project.

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

#include "icon.h"
#include "icon_p.h"


namespace Cagibi
{
bool Icon::isKey( const QString& key ) { return IconPrivate::isKey(key); }


Icon::Icon()
  : d( new IconPrivate() )
{
}

Icon::Icon( IconPrivate* _d )
  : d( _d )
{
}

Icon::Icon( const Icon& other )
  : d( other.d )
{
}


const QString& Icon::mimeType() const { return d->mimeType(); }
int Icon::width() const { return d->width(); }
int Icon::height() const { return d->height(); }
int Icon::depth() const { return d->depth(); }
const QString& Icon::url() const { return d->url(); }

void Icon::setProperty( const QString& key, const QString& value )
{
    d->setProperty( key, value );
};


Icon& Icon::operator=( const Icon& other )
{
    d = other.d;
    return *this;
}

bool Icon::operator==( const Icon& other ) const
{
    return ( d == other.d );
}

Icon::~Icon()
{
}

}
