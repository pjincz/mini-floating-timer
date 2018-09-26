#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QAction>
#include <QSoundEffect>

#include "volumeaction.h"

class Global : public QObject
{
    Q_OBJECT
public:
    explicit Global(QObject *parent = nullptr);

    VolumeAction * volumeAction();

public:
    static Global *instance();

protected slots:
    void volumeChanged(float v);

private:
    static Global *s_instance;

    VolumeAction * m_volumeAction = nullptr;
    QSoundEffect * m_soundEffect = nullptr;
};

#endif // GLOBAL_H
