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

#ifndef Phonon_GSTREAMER_AUDIOEFFECT_H
#define Phonon_GSTREAMER_AUDIOEFFECT_H

#include "effect.h"

#include <gst/gstelement.h>

#ifndef QT_NO_PHONON_EFFECT

class QObject;

namespace Phonon
{
namespace Gstreamer
{

class AudioOutput;
class EffectInfo;

class AudioEffect : public Effect
{
    Q_OBJECT
public:
    AudioEffect (Backend *backend, int effectId, QObject *parent);

protected:
    GstElement* createEffectBin();

    GstElement* audioElement() const {
        return effectBin();
    }

private:
    QString m_effectName;

};

} // namespace Gstreamer
} // namespace Phonon


#endif //QT_NO_PHONON_EFFECT

#endif // Phonon_GSTREAMER_AUDIOEFFECT_H
