#include "setupDlg.h"
#include "ui_setupDlg.h"
#include "QMessageBox"
#include "QSettings"
#include "QDesktopServices"
#include "QUrl"

const char * HELP_LINK = "https://github.com/jinchizhong/mini-floating-timer/blob/master/README.md";

SetupDlg::SetupDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowStaysOnTopHint),
    ui(new Ui::SetupDlg)
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);

    QSettings s;
    QString t = s.value("defaultTime", "00:30:00").toString();
    ui->lineEdit->setText(t);
    ui->lineEdit->setFocus();
    ui->lineEdit->setCursorPosition(0);
}

SetupDlg::~SetupDlg()
{
    delete ui;
}

void SetupDlg::accept()
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
    m_total *= 1000;

    if (ui->updateDefault->checkState()) {
        QSettings s;
        s.setValue("defaultTime", ui->lineEdit->text());
    }

    QDialog::accept();
}

void SetupDlg::setTotal(qint64 total)
{
    QString x = QString("%1:%2:%3")
            .arg(total / 1000 / 3600, 2, 10, QLatin1Char('0'))
            .arg(total / 1000 % 3600 / 60, 2, 10, QLatin1Char('0'))
            .arg(total / 1000 % 60, 2, 10, QLatin1Char('0'));
    ui->lineEdit->setText(x);
}

void SetupDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Help) {
        QDesktopServices::openUrl(QUrl(HELP_LINK));
    }
}
