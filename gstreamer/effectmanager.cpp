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

#include "effectmanager.h"
#include "phonon-config-gstreamer.h"
#include <gst/gst.h>

#include "backend.h"
#include "gsthelper.h"

/*
 * This class manages the list of currently
 * available audio effects.
 */

namespace Phonon
{
namespace Gstreamer
{

EffectInfo::EffectInfo(const QString &name, const QString &description,
                       const QString &author)
        : m_name(name)
        , m_description(description)
        , m_author(author)
{
}

EffectManager::EffectManager(Backend *backend)
        : QObject(backend)
        , m_backend(backend)
{
    GList *factoryList = gst_registry_get_feature_list(gst_registry_get(), GST_TYPE_ELEMENT_FACTORY);

    QString name;
    QString klass;
    QString description;
    QString author;
    for (GList* iter = g_list_first(factoryList); iter != NULL ; iter = g_list_next(iter)) {
        GstPluginFeature *feature = GST_PLUGIN_FEATURE(iter->data);
        klass = gst_element_factory_get_klass(GST_ELEMENT_FACTORY(feature));
        if (klass == QLatin1String("Filter/Effect/Audio")) {
            name = GST_OBJECT_NAME(feature);

            // These plugins simply make no sense to the frontend:
            // "audiorate" Should be internal
            // "volume" not needed
            // "equalizer-nbands" not really useful at the moment

            // These plugins simply don't work or have major stability issues:
            // "iir" Does not seem to do much at the moment
            // "audioinvert" Only works for some streams, should be invesigated
            // "lpwsinc" Crashes for large values of filter kernel
            // "name" Crashes for large values of filter kernel

            // Seems to be working, but not well tested:
            // name == "rglimiter" Seems functional
            // name == "rgvolume" Seems to be working

            QString pluginString = qgetenv("PHONON_GST_ALL_EFFECTS");
            bool acceptAll = pluginString.toInt();

            if (acceptAll
                // Plugins that have been accepted so far
                 || name == QLatin1String("audiopanorama")
                 || name == QLatin1String("audioamplify")
                 || name == QLatin1String("audiodynamic")
                 || name == QLatin1String("equalizer-10bands")
                 || name == QLatin1String("speed"))
                {
                    description = gst_element_factory_get_description(GST_ELEMENT_FACTORY(feature));
                    author = gst_element_factory_get_author(GST_ELEMENT_FACTORY(feature));
                    EffectInfo *effect = new EffectInfo(name, description, author);
                    m_audioEffectList.append(effect);

#ifdef __GNUC__
#warning TODO - get rid of equalizer name mapping (also see audioeffect.cpp)
#endif
                    // Map the GStreamer name to the name used by Xine, to allow
                    // API consumers that think KEqualizer is a persistant name
                    // to have a working equalizer with GStreamer too (e.g. Amarok).
                    if (name == QLatin1String("equalizer-10bands")) {
                        m_audioEffectList.append(new EffectInfo(
                                                     QLatin1String("KEqualizer"),
                                                     QLatin1String("Compatibility effect. Do not use in new software!"),
                                                     author));
                    }
            }
        }
    }
    gst_plugin_feature_list_free(factoryList);
}

EffectManager::~EffectManager()
{
    qDeleteAll(m_audioEffectList);
    m_audioEffectList.clear();
}

/**
  * Returns a list of available audio effects
  */
const QList<EffectInfo*> EffectManager::audioEffects() const
{
    return m_audioEffectList;
}

}
}
