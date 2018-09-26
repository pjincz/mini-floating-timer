#include "volumeaction.h"
#include <QFormLayout>
#include <QSlider>
#include <QStyle>
#include <QStyleOption>
#include <math.h>

const int SLIDER_MAX = 30;
const float VOLUME_MIN = 0.01;
const float VOLUME_MAX = 1;

float sliderValueToFloat(int x) {
    if (x == 0)
        return 0;

    return exp((log(VOLUME_MAX) - log(VOLUME_MIN)) / (SLIDER_MAX - 1) * (x - 1) + log(VOLUME_MIN));
}

int floatToSliderValue(float x) {
    if (x < VOLUME_MIN)
        return 0;

    double tmp = (log(x) - log(VOLUME_MIN)) / (log(VOLUME_MAX) - log(VOLUME_MIN)) * (SLIDER_MAX - 1);

    if (tmp > SLIDER_MAX)
        return SLIDER_MAX;
    return (int)(tmp + 1.45);
}

VolumeAction::VolumeAction(float initVolume, QObject *parent)
    : QWidgetAction(parent), m_volume(initVolume)
{
}

QWidget *VolumeAction::createWidget(QWidget *parent)
{
    QWidget * w = new QWidget(parent);
    QFormLayout * layout = new QFormLayout(w);

    QStyleOption opt;
    int hmargin = w->style()->pixelMetric(QStyle::PM_MenuHMargin, &opt, w);
    int vmargin = w->style()->pixelMetric(QStyle::PM_MenuVMargin, &opt, w);
    int icone = w->style()->pixelMetric(QStyle::PM_SmallIconSize, &opt, w);
    int fw = w->style()->pixelMetric(QStyle::PM_MenuPanelWidth, &opt, w);
    int spacing = w->style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing, &opt, w);
    // following '4' is hardcored in qt,
    layout->setContentsMargins(fw + hmargin + icone + 4 + spacing, vmargin + fw, hmargin + fw, vmargin + fw);

    QSlider * slider = new QSlider(Qt::Horizontal, w);
    slider->setRange(0, SLIDER_MAX);
    slider->setValue(floatToSliderValue(m_volume));
    slider->setTracking(false);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slideChanged(int)));

    layout->addRow("Volume:", slider);

    return w;
}

float VolumeAction::volume()
{
    return m_volume;
}

void VolumeAction::setVolume(float v)
{
    m_volume = v;

    int sliderPos = floatToSliderValue(v);

    for (QWidget * w: createdWidgets()) {
        QSlider * s = w->findChild<QSlider*>();
        if (s->value() != v)
            s->setValue(sliderPos);
    }

    emit volumeChanged(v);
}

void VolumeAction::slideChanged(int val)
{
    setVolume(sliderValueToFloat(val));
}
