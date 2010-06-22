#ifndef PHONON_TEST_H
#define PHONON_TEST_H

#include <Qt/QtCore>
#include <Qt/QtGui>
#include <KDE/KUrl>
#include <phonon/AudioOutput>
#include <phonon/VideoPlayer>
#include <phonon/VideoWidget>
#include <phonon/VolumeSlider>
#include <phonon/MediaObject>
#include <phonon/Global>

class MediaPlayer : public QWidget
{
    Q_OBJECT

    public:
        MediaPlayer(QWidget *parent);
        ~MediaPlayer();

    public slots:
        void setDeviceName( const QString& device);

    private:
        void updateDeviceList();

    private:
        Phonon::MediaObject *m_media;
        Phonon::AudioOutput *m_aoutput;
        Phonon::VideoWidget *m_vwidget;
        Phonon::VolumeSlider *m_volumeSlider;

        QPushButton *m_playButton;
        QComboBox *m_deviceNameCombo;
};


#endif



