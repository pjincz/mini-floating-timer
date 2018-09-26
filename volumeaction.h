#ifndef VOLUMEACTION_H
#define VOLUMEACTION_H

#include <QWidgetAction>

class VolumeAction : public QWidgetAction
{
    Q_OBJECT
public:
    VolumeAction(float initVolume, QObject *parent = nullptr);

    QWidget *createWidget(QWidget *parent);

    Q_PROPERTY(float volume READ volume WRITE setVolume)
    float volume();
    void setVolume(float v);

protected slots:
    void slideChanged(int val);

signals:
    void volumeChanged(float x);

private:
    float m_volume;
};

#endif // VOLUMEACTION_H
