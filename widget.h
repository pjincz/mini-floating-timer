#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDateTime>

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

    void paintEvent(QPaintEvent * e);
    void timerEvent(QTimerEvent *e);

private:
    QPoint m_oldPos;
    qint64 m_dest;
    qint64 m_total;
};

#endif // WIDGET_H
