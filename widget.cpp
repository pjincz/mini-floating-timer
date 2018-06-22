#include "widget.h"
#include "QMouseEvent"
#include "QPainter"
#include "QDateTime"
#include "QApplication"
#include "QStyle"
#include "QSoundEffect"
#include "QFileInfo"
#include "QDesktopServices"
#include "setupDlg.h"
#include "QLabel"

const char * GITHUB_LINK = "https://github.com/jinchizhong/mini-floating-timer";

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

Widget::Widget(qint64 total) :
    QWidget(0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),
    m_oldPos(-1, -1),
    m_total(total),
    m_soundEffect(0)
{
    m_menu = new QMenu(this);

    m_menu->addAction(QIcon(":/assets/github.png"), GITHUB_LINK, this, SLOT(showGithub()));
    m_menu->addSeparator();

    m_menu->addAction(style()->standardIcon(QStyle::SP_MediaPause), "&Pause/Resume", this, SLOT(togglePause()));
    m_menu->addAction("Restart", this, SLOT(restartTimer()));
    m_menu->addAction("&Reset...", this, SLOT(resetTimer()));
    m_menu->addSeparator();

    for (int i = 0; i < sizeof(COLORLIST) / sizeof(*COLORLIST); ++i) {
        QPixmap pmp(32, 32);
        pmp.fill(QColor(COLORLIST[i]));
        QIcon icon(pmp);
        m_menu->addAction(icon, QString("Change color to #%1").arg(i), this, SLOT(changeColor()))->setData(i);
    }
    m_menu->addSeparator();
    m_menu->addAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), "&Quit", qApp, SLOT(quit()));

    setWindowOpacity(0.8);
    resize(120, this->fontInfo().pixelSize());

    m_start = QDateTime::currentMSecsSinceEpoch();
    m_lapse = -1;
    m_color = QColor(COLORLIST[m_start % 10]);

    startTimer(100);
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
        togglePause();
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

void Widget::keyReleaseEvent(QKeyEvent *e)
{
    m_keySeq = (m_keySeq + e->text()).right(4);
    if (m_keySeq.toLower() == "meow") {
        QLabel * l = new QLabel();
        l->setPixmap(QPixmap(":/assets/meow.jpg"));
        l->show();
    }
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter pt(this);

    int rem = m_lapse < 0 ? m_total - (QDateTime::currentMSecsSinceEpoch() - m_start) : m_total - m_lapse;
    if (rem > 0)
    {
        double rate = 1 - rem * 1.0 / m_total;

        QRect rc = rect();
        rc.setWidth(rc.width() * rate);
        pt.fillRect(rc, m_color);

        QString remain = QString("%1:%2:%3.%4")
                .arg(rem / 1000 / 3600, 2, 10, QLatin1Char('0'))
                .arg(rem / 1000  % 3600 / 60, 2, 10, QLatin1Char('0'))
                .arg(rem / 1000  % 60, 2, 10, QLatin1Char('0'))
                .arg(rem % 1000 / 100);
        pt.drawText(rect(), Qt::AlignCenter, remain);
    }
    else
    {
        bool highlight = QDateTime::currentMSecsSinceEpoch() % 1000 > 500;
        pt.fillRect(rect(), highlight ? Qt::red : Qt::white);

        pt.drawText(rect(), Qt::AlignCenter, "00:00:00");
    }
}

void Widget::timerEvent(QTimerEvent *)
{
    int rem = m_lapse < 0 ? m_total - (QDateTime::currentMSecsSinceEpoch() - m_start) : m_total - m_lapse;
    if (rem <= 0 && !m_soundEffect)
    {
        m_soundEffect = new QSoundEffect(this);

        m_soundEffect->setSource(QUrl("qrc:/assets/clock.wav"));
        m_soundEffect->setLoopCount(20);
        m_soundEffect->setVolume(0.5);
        m_soundEffect->play();
    }

    update();
}

void Widget::changeColor()
{
    QAction * a = qobject_cast<QAction*>(sender());
    this->m_color = QColor(COLORLIST[a->data().toInt()]);
    update();
}

void Widget::togglePause()
{
    if (m_lapse < 0) {
        // pause
        m_lapse = QDateTime::currentMSecsSinceEpoch() - m_start;
        m_start = -1;
    } else {
        // resume
        m_start = QDateTime::currentMSecsSinceEpoch() - m_lapse;
        m_lapse = -1;
    }
    update();
}

void Widget::showGithub()
{
    QDesktopServices::openUrl(QUrl(GITHUB_LINK));
}

void Widget::resetTimer()
{
    if (m_soundEffect)
        m_soundEffect->stop();

    SetupDlg dlg;
    dlg.setTotal(m_total);

    if (!dlg.exec())
        return;

    Widget * w = new Widget(dlg.m_total);
    w->move(this->pos());
    w->m_color = this->m_color;
    w->show();
    deleteLater();
}

void Widget::restartTimer()
{
    if (m_soundEffect)
        m_soundEffect->stop();

    Widget * w = new Widget(m_total);
    w->move(this->pos());
    w->m_color = this->m_color;
    w->show();
    deleteLater();
}
