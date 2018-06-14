#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QMenu>

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

private:
    QPoint m_oldPos;
    qint64 m_dest;
    qint64 m_total;

    bool m_highlight;
    int m_colorIdx;

    QMenu * m_menu;
};

#endif // WIDGET_H
