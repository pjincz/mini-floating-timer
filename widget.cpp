#include "widget.h"
#include "QMouseEvent"
#include "QPainter"
#include "QDateTime"

Widget::Widget(qint64 total) :
    QWidget(0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),
    m_oldPos(-1, -1),
    m_total(total),
    m_highlight(false)
{
    setWindowOpacity(0.75);
    resize(120, 9);

    m_dest = QDateTime::currentSecsSinceEpoch() + m_total;

    startTimer(500);
}

Widget::~Widget()
{
}

void Widget::mousePressEvent(QMouseEvent * e)
{
    m_oldPos = e->globalPos();
}

void Widget::mouseReleaseEvent(QMouseEvent*)
{
    m_oldPos.setX(-1);
}

void Widget::mouseMoveEvent(QMouseEvent * e)
{
    if (m_oldPos.x() >= 0) {
        QPoint delta = e->globalPos() - m_oldPos;
        QPoint pt = pos() + delta;
        if (pt.x() < 0)
            pt.setX(0);
        if (pt.y() < 0)
            pt.setY(0);
        move(pt);
        m_oldPos = e->globalPos();
    }
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter pt(this);

    int rem = m_dest -  QDateTime::currentSecsSinceEpoch();
    if (rem > 0)
    {
        double rate = 1 - (m_dest -  QDateTime::currentSecsSinceEpoch()) * 1.0 / m_total;

        QRect rc = rect();
        rc.setWidth(rc.width() * rate);
        pt.fillRect(rc, Qt::green);

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
