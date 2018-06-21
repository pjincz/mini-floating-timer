#ifndef INITSETTING_H
#define INITSETTING_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class InitSetting;
}

class InitSetting : public QDialog
{
    Q_OBJECT

public:
    explicit InitSetting(QWidget *parent = 0);
    ~InitSetting();

    void accept();

    qint64 m_total;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::InitSetting *ui;
};

#endif // INITSETTING_H
