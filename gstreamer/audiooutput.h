/*  This file is part of the KDE project.

    Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
    Copyright (C) 2008 Matthias Kretz <kretz@kde.org>

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

#ifndef Phonon_GSTREAMER_AUDIOOUTPUT_H
#define Phonon_GSTREAMER_AUDIOOUTPUT_H

#include "medianode.h"

#include <phonon/audiooutputinterface.h>

namespace Phonon
{
namespace Gstreamer
{
class AudioOutput : public QObject, public AudioOutputInterface, public MediaNode
{
    Q_OBJECT
    Q_INTERFACES(Phonon::AudioOutputInterface Phonon::Gstreamer::MediaNode)
public:
    AudioOutput(Backend *backend, QObject *parent);
    ~AudioOutput();

    qreal volume() const;
    int outputDevice() const;
    void setVolume(qreal newVolume);
    bool setOutputDevice(int newDevice);

#if (PHONON_VERSION >= PHONON_VERSION_CHECK(4, 2, 0))
    bool setOutputDevice(const AudioOutputDevice &newDevice);
#endif

#if (PHONON_VERSION >= PHONON_VERSION_CHECK(4, 6, 50))
    void setStreamUuid(QString uuid);
#endif

public:
    GstElement *audioElement() const
    {
        Q_ASSERT(m_audioBin);
        return m_audioBin;
    }

Q_SIGNALS:
    void volumeChanged(qreal newVolume);
    void audioDeviceFailed();

private:
    bool setOutputDevice(const QByteArray &, const QString &, const GstState);

private:
    qreal m_volumeLevel;
    int m_device;

    GstElement *m_volumeElement;
    GstElement *m_audioBin;
    GstElement *m_audioSink;
    GstElement *m_conv;

    QString m_streamUuid;
};
}
} //namespace Phonon::Gstreamer

#endif // Phonon_GSTREAMER_AUDIOOUTPUT_H
