#ifndef ADD_PACKAGE_H
#define ADD_PACKAGE_H

#include <QWidget>

namespace Ui {
class addPackage;
}

class addPackage : public QWidget
{
    Q_OBJECT

public:
    explicit addPackage(QWidget *parent = 0);
    ~addPackage();

private:
    Ui::addPackage *ui;

private slots:
    void on_addPackage_BT_clicked();
signals:
    void addPackageSignal(QString,bool);
};

#endif // ADD_PACKAGE_H
