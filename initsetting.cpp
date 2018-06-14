#include "initsetting.h"
#include "ui_initsetting.h"
#include "QMessageBox"

InitSetting::InitSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitSetting),
    m_total(-1)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
    ui->lineEdit->setCursorPosition(0);
}

InitSetting::~InitSetting()
{
    delete ui;
}

void InitSetting::accept()
{
    QString str = ui->lineEdit->text();
    QStringList sl = str.split(":");
    if (sl.length() == 3) {
        m_total = sl[0].toInt() * 3600 + sl[1].toInt() * 60 + sl[2].toInt();
    } else if (sl.length() == 2) {
        m_total = sl[0].toInt() * 60 + sl[1].toInt();
    } else if (sl.length() == 1) {
        m_total = sl[0].toInt();
    } else {
        QMessageBox::critical(this, "Bad time format", "Bad time format");
        return;
    }

    QDialog::accept();
}
