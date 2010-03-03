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
#include <QtCore/QString>


namespace UPnP
{

class Icon
{
  public:
    static const char* const keys[];
    enum { Mimetype=0, Width, Height, Depth, Url, KeyCount };

    static bool isKey( const QString& key );

  public:
    Icon();

  public:
    const QString& mimeType() const;
    int width() const;
    int height() const;
    int depth() const;
    QString url() const;

  public:
    void setProperty( const QString& key, const QString& value );

  protected:
    QString mMimeType;
    int mWidth;
    int mHeight;
    int mDepth;
    QString mUrl;
};


inline Icon::Icon() : mWidth( 0 ), mHeight( 0 ), mDepth( 0 ) {}

inline const QString& Icon::mimeType() const { return mMimeType; }
inline int Icon::width() const { return mWidth; }
inline int Icon::height() const { return mHeight; }
inline int Icon::depth() const { return mDepth; }
inline QString Icon::url() const { return mUrl; }

inline void Icon::setProperty( const QString& key, const QString& value )
{
    if( key == keys[Mimetype] )
        mMimeType = value;
    else if( key == keys[Width] )
        mWidth = value.toInt();
    else if( key == keys[Height] )
        mHeight = value.toInt();
    else if( key == keys[Depth] )
        mDepth = value.toInt();
    else if( key == keys[Url] )
        mUrl == value;
}

inline bool Icon::isKey( const QString& key )
{
    bool result = false;

    for( int i=0; i<KeyCount; ++i )
        if( key == QLatin1String(keys[i]) )
        {
            result = true;
            break;
        }

    return result;
}

}

#endif
