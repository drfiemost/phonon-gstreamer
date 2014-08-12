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

#include "widgetrenderer.h"

#include <gst/gst.h>
#include "backend.h"
#include "debug.h"
#include "mediaobject.h"
#include "qwidgetvideosink.h"
#include "videowidget.h"
#include "qrgb.h"

#include <QtCore/QTime>
#include <QtGui/QPainter>

// support old OpenGL installations (1.2)
// assume that if TEXTURE0 isn't defined, none are
#ifndef GL_TEXTURE0
# define GL_TEXTURE0    0x84C0
# define GL_TEXTURE1    0x84C1
# define GL_TEXTURE2    0x84C2
#endif

#ifndef QT_NO_PHONON_VIDEO
static void frameRendered()
{
    static QString displayFps = qgetenv("PHONON_GST_FPS");
    if (displayFps.isEmpty()) {
        return;
    }

    static int frames = 0;
    static QTime lastTime = QTime::currentTime();
    QTime time = QTime::currentTime();

    int delta = lastTime.msecsTo(time);
    if (delta > 2000) {
        printf("FPS: %f\n", 1000.0 * frames / qreal(delta));
        lastTime = time;
        frames = 0;
    }

    ++frames;
}

namespace Phonon
{
namespace Gstreamer
{

WidgetRenderer::WidgetRenderer(VideoWidget *videoWidget_)
        : AbstractRenderer(videoWidget_)
        , m_width(0)
        , m_height(0)
{
    debug() << "Creating QWidget renderer";
    GstElement *videoSink = GST_ELEMENT(g_object_new(get_type_RGB(), NULL));
    if (videoSink) {
        setVideoSink(videoSink);

        QWidgetVideoSinkBase*  sink = reinterpret_cast<QWidgetVideoSinkBase*>(videoSink);
        // Let the videosink know which widget to direct frame updates to
        sink->renderWidget = videoWidget();
    }

    // Clear the background with black by default
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::black);
    videoWidget()->setPalette(palette);
    videoWidget()->setAutoFillBackground(true);
    videoWidget()->setAttribute(Qt::WA_NoSystemBackground, false);
    videoWidget()->setAttribute(Qt::WA_PaintOnScreen, false);
}

WidgetRenderer::~WidgetRenderer()
{
}


void WidgetRenderer::setNextFrame(const QByteArray &array, int w, int h)
{
    if (videoWidget()->root()->state() == Phonon::LoadingState) {
        return;
    }

    m_frame = QImage((uchar *)array.constData(), w, h, QImage::Format_RGB32);
    m_array = array;
    m_width = w;
    m_height = h;

    videoWidget()->update();
}

void WidgetRenderer::clearFrame()
{
    m_frame = QImage();
    m_array = QByteArray();
    videoWidget()->update();
}

const QImage &WidgetRenderer::currentFrame() const
{
    return m_frame;
}

void WidgetRenderer::handlePaint(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(videoWidget());
    m_drawFrameRect = videoWidget()->calculateDrawFrameRect();
    painter.drawImage(drawFrameRect(), currentFrame());
    frameRendered();
}

bool WidgetRenderer::eventFilter(QEvent * event)
{
    if (event->type() == QEvent::User) {
        NewFrameEvent *frameEvent= static_cast <NewFrameEvent *>(event);
        setNextFrame(frameEvent->frame, frameEvent->width, frameEvent->height);
        return true;
    }
    return false;
}

}
} //namespace Phonon::Gstreamer
#endif //QT_NO_PHONON_VIDEO
