#include "widget.h"
#include "initsetting.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InitSetting w;
    w.exec();

    if (w.m_total > 0) {
        Widget timer(w.m_total);
        timer.show();
        return a.exec();
    }

    return 0;
}
