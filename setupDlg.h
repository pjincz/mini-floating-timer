#ifndef SETUPDLG_H
#define SETUPDLG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class SetupDlg;
}

class SetupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDlg(QWidget *parent = 0);
    ~SetupDlg();

    void accept();

    void setTotal(qint64 total);

    qint64 m_total;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SetupDlg *ui;
};

#endif // SETUPDLG_H
