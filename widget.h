#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QMenu>

class QSoundEffect;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(qint64 total);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void wheelEvent(QWheelEvent * e);

    void paintEvent(QPaintEvent * e);
    void timerEvent(QTimerEvent *e);

public slots:
    void changeColor();
    void togglePause();
    void showGithub();
    void resetTimer();
    void restartTimer();

private:
    qint64 m_total;
    qint64 m_start; // only valid in running status, or -1
    qint64 m_lapse; // only valid in pause status, or -1

    QPoint m_oldPos;

    QColor m_color;

    QSoundEffect * m_soundEffect;

    QMenu * m_menu;
};

#endif // WIDGET_H
