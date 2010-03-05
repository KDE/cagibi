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

#ifndef ICON_H
#define ICON_H

// Qt
#include <QtCore/QExplicitlySharedDataPointer>

class QString;


namespace UPnP
{
class IconPrivate;


class Icon
{
  friend class DeviceDescriptionXmlHandler;

  public:
    static bool isKey( const QString& key );

  protected:
    explicit Icon( IconPrivate* _d );

  public:
    Icon();
    Icon( const Icon& other );
    ~Icon();

  public:
    Icon& operator=( const Icon& other );
    bool operator==( const Icon& other ) const;

  public:
    QString mimeType() const;
    int width() const;
    int height() const;
    int depth() const;
    QString url() const;

  public:
    void setProperty( const QString& key, const QString& value );

  protected:
    QExplicitlySharedDataPointer<IconPrivate> d;
};

}

#endif