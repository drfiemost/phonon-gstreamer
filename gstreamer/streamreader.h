/*  This file is part of the KDE project.

Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).

This library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 or 3 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PHONON_IODEVICEREADER_H
#define PHONON_IODEVICEREADER_H

#include <phonon/streaminterface.h>

#include "mediaobject.h"

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PHONON_ABSTRACTMEDIASTREAM

namespace Phonon
{

class MediaSource;

namespace Gstreamer
{

class StreamReader : public QObject, Phonon::StreamInterface
{
public:
    StreamReader(const Phonon::MediaSource &source, MediaObject *parent);

    /*
     * Overloads for StreamInterface
     */
    int currentBufferSize() const;
    void writeData(const QByteArray &data);
    void setCurrentPos(qint64 pos);
    quint64 currentPos() const;
    GstFlowReturn read(quint64 offset, int length, char * buffer);
    void endOfData();
    void setStreamSize(qint64 newSize);
    qint64 streamSize() const;
    void setStreamSeekable(bool seekable);
    bool streamSeekable() const ;

private:
    quint64 m_pos;
    quint64 m_size;
    bool m_eos;
    bool m_seekable;
    MediaObject *m_mediaObject;
    QByteArray m_buffer;
    QMutex m_mutex;
    QWaitCondition m_waitingForData;
};

}
}

#endif //QT_NO_PHONON_ABSTRACTMEDIASTREAM

QT_END_NAMESPACE

#endif
