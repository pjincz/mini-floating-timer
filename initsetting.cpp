#include "initsetting.h"
#include "ui_initsetting.h"
#include "QMessageBox"
#include "QSettings"
#include "QDesktopServices"
#include "QUrl"

extern const char * GITHUB_LINK;

InitSetting::InitSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitSetting),
    m_total(-1)
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);

    QSettings s;
    QString t = s.value("defaultTime", "00:30:00").toString();
    ui->lineEdit->setText(t);
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

    if (ui->updateDefault->checkState()) {
        QSettings s;
        s.setValue("defaultTime", ui->lineEdit->text());
    }

    QDialog::accept();
}

void InitSetting::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Help) {
        QDesktopServices::openUrl(QUrl(GITHUB_LINK));
    }
}
