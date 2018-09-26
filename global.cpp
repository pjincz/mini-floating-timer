#include "global.h"
#include <QSettings>
#include <QSoundEffect>

Global * Global::s_instance;

Global::Global(QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(!s_instance);

    s_instance = this;
}

VolumeAction *Global::volumeAction()
{
    if (!m_volumeAction) {
        QSettings s;
        float volume = s.value("volume", 0.5).toFloat();
        m_volumeAction = new VolumeAction(volume, this);

        connect(m_volumeAction, SIGNAL(volumeChanged(float)), this, SLOT(volumeChanged(float)));
    }

    return m_volumeAction;
}

Global *Global::instance()
{
    Q_ASSERT(s_instance);
    return s_instance;
}

void Global::volumeChanged(float v)
{
    QSettings s;
    s.setValue("volume", v);

    if (!m_soundEffect) {
        m_soundEffect = new QSoundEffect(this);
        m_soundEffect->setSource(QUrl("qrc:/assets/clock.wav"));
        m_soundEffect->setLoopCount(1);
    }

    m_soundEffect->setVolume(v);
    m_soundEffect->play();
}
