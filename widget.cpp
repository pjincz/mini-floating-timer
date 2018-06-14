#include "widget.h"
#include "QMouseEvent"
#include "QPainter"
#include "QDateTime"
#include "QApplication"
#include "QStyle"

Widget::Widget(qint64 total) :
    QWidget(0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),
    m_oldPos(-1, -1),
    m_total(total),
    m_highlight(false)
{
    m_menu = new QMenu(this);
    m_menu->addAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), "&Quit", qApp, SLOT(quit()));

    setWindowOpacity(0.8);
    resize(120, this->fontInfo().pixelSize());

    m_dest = QDateTime::currentSecsSinceEpoch() + m_total;

    m_colorIdx = QDateTime::currentMSecsSinceEpoch() % 10;

    startTimer(500);
}

Widget::~Widget()
{
}

void Widget::mousePressEvent(QMouseEvent * e)
{
    if (e->buttons() & Qt::LeftButton)
        m_oldPos = e->globalPos();
}

void Widget::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->button() == Qt::RightButton)
    {
        m_menu->popup(e->globalPos());
    }
    if (e->button() == Qt::MiddleButton)
    {
        m_colorIdx = (m_colorIdx + 1) % 10;
        update();
    }
}

void Widget::mouseMoveEvent(QMouseEvent * e)
{
    if (e->buttons() & Qt::LeftButton) {
        QPoint delta = e->globalPos() - m_oldPos;
        QPoint pt = pos() + delta;
        if (pt.x() < 0)
            pt.setX(0);
        if (pt.y() < 1)
            pt.setY(1);
        move(pt);
    }
    m_oldPos = e->globalPos();
}

void Widget::wheelEvent(QWheelEvent *e)
{
    double opacity = windowOpacity();
    opacity += e->angleDelta().y() / 1200.0;
    if (opacity > 1)
        opacity = 1;
    if (opacity < 0.15)
        opacity = 0.15;
    setWindowOpacity(opacity);
}

const char * COLORLIST[] = {
    "#1f77b4",
    "#ff7f0e",
    "#2ca02c",
    "#d62728",
    "#9467bd",
    "#8c564b",
    "#e377c2",
    "#7f7f7f",
    "#bcbd22",
    "#17becf"
};

void Widget::paintEvent(QPaintEvent *)
{
    QPainter pt(this);

    int rem = m_dest -  QDateTime::currentSecsSinceEpoch();
    if (rem > 0)
    {
        double rate = 1 - (m_dest -  QDateTime::currentSecsSinceEpoch()) * 1.0 / m_total;

        QRect rc = rect();
        rc.setWidth(rc.width() * rate);
        pt.fillRect(rc, QColor(COLORLIST[m_colorIdx]));

        QString remain = QString("%1:%2:%3")
                .arg(rem / 3600, 2, 10, QLatin1Char('0'))
                .arg(rem % 3600 / 60, 2, 10, QLatin1Char('0'))
                .arg(rem % 60, 2, 10, QLatin1Char('0'));
        pt.drawText(rect(), Qt::AlignCenter, remain);
    }
    else
    {
        pt.fillRect(rect(), m_highlight ? Qt::red : Qt::white);

        pt.drawText(rect(), Qt::AlignCenter, "00:00:00");
    }
}

void Widget::timerEvent(QTimerEvent *)
{
    m_highlight = !m_highlight;
    update();
}
