/*  This file is part of the KDE project.

    Copyright (C) 2    //Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).007 Nokia Corporation and/or its subsidiary(-ies).
    Copyright (C) 2012 Anssi Hannula <anssi.hannula@iki.fi>

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

#ifndef Phonon_GSTREAMER_VIDEOWIDGET_H
#define Phonon_GSTREAMER_VIDEOWIDGET_H

#include <phonon/videowidgetinterface.h>

#include "medianode.h"

#ifndef QT_NO_PHONON_VIDEO
class QString;

namespace Phonon
{
namespace Gstreamer
{

class AbstractRenderer;
class Backend;

class VideoWidget : public QWidget, public Phonon::VideoWidgetInterface44, public MediaNode
{
    Q_OBJECT
    Q_INTERFACES(Phonon::VideoWidgetInterface44 Phonon::Gstreamer::MediaNode)
public:
    explicit VideoWidget(Backend *backend, QWidget *parent = 0);
    ~VideoWidget();

    void setupVideoBin();
    void paintEvent(QPaintEvent *event);
    void setVisible(bool);

    Phonon::VideoWidget::AspectRatio aspectRatio() const;
    void setAspectRatio(Phonon::VideoWidget::AspectRatio aspectRatio);
    Phonon::VideoWidget::ScaleMode scaleMode() const;
    void setScaleMode(Phonon::VideoWidget::ScaleMode);
    qreal brightness() const;
    void setBrightness(qreal);
    qreal contrast() const;
    void setContrast(qreal);
    qreal hue() const;
    void setHue(qreal);
    qreal saturation() const;
    void setSaturation(qreal);
    QSize sizeHint() const;
    QRect scaleToAspect(QRect srcRect, int w, int h) const;
    QRect calculateDrawFrameRect() const;
    QImage snapshot() const;

    QSize movieSize() const {
        return m_movieSize;
    }

    GstElement* videoElement() const {
        return m_videoBin;
    }

    bool event(QEvent *);

    QWidget *widget() {
        return this;
    }

    static void cb_capsChanged(GstPad *pad, GParamSpec *spec, gpointer data);

    void finalizeLink();
    void prepareToUnlink();

public slots:
    void setMovieSize(const QSize &size);
    void mouseOverActive(bool active);
    void syncX();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    GstElement *m_videoBin;
    QSize m_movieSize;
    AbstractRenderer *m_renderer;

private slots:
    void updateWindowID();

private:
    Phonon::VideoWidget::AspectRatio m_aspectRatio;
    qreal m_brightness, m_hue, m_contrast, m_saturation;
    Phonon::VideoWidget::ScaleMode m_scaleMode;

    GstElement *m_videoBalance;
    GstElement *m_colorspace;
    GstElement *m_videoplug;
};

}
} //namespace Phonon::Gstreamer
#endif //QT_NO_PHONON_VIDEO
#endif // Phonon_GSTREAMER_VIDEOWIDGET_H
