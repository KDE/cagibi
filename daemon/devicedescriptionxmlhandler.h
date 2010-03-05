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

#ifndef DEVICEDESCRIPTIONXMLHANDLER_H
#define DEVICEDESCRIPTIONXMLHANDLER_H

// lib
#include "icon.h"
#include "service.h"
#include "device.h"
// Qt
#include <QtXml/QXmlDefaultHandler>
#include <QtCore/QStack>


namespace UPnP
{
class RootDevice;


class DeviceDescriptionXMLHandler : public QXmlDefaultHandler
{
    enum Element
    {
        TopLevel,
        RootElement,
            UrlBaseElement,
            DeviceElement,
                IconListElement,
                    IconElement,
                ServiceListElement,
                    ServiceElement,
                        DataElement,
                DeviceListElement,
                    // here recursively a DeviceElement
        UnknownElement
    };

  public:
    DeviceDescriptionXMLHandler( RootDevice* device );

    virtual ~DeviceDescriptionXMLHandler();

  public: // QXmlDefaultHandler API
    virtual bool startDocument();
    virtual bool endDocument();
    virtual bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName,
                               const QXmlAttributes& attributes );
    virtual bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );
    virtual bool characters( const QString& characters );

  protected:
    RootDevice* mRootDevice;

    Icon mCurrentIcon;
    Service mCurrentService;
    Device mCurrentDevice;

    QString mBaseUrl;
    QString mCharacterData;
    QStack<Element> mStatusStack;
};

}

#endif
