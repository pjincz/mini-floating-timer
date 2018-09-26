#include "widget.h"
#include "setupDlg.h"
#include "global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Global g;

    a.setOrganizationName("Chizhong Jin");
    a.setApplicationName("mini-floating-timer");

    SetupDlg dlg;
    dlg.exec();

    if (dlg.m_total > 0) {
        Widget * w = new Widget(dlg.m_total);
        w->show();
        return a.exec();
    }

    return 0;
}
