#ifndef ADD_PATIENT_H
#define ADD_PATIENT_H

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include "myTcpsocket.h"

namespace Ui {
class addPatient;
}

class addPatient : public QWidget
{
    Q_OBJECT

public:
    explicit addPatient(QWidget *parent = 0, QString userName = NULL, MyTcpsocket *socket = NULL);
    ~addPatient();

private:
    Ui::addPatient *ui;
    void updatePackageFunc();
    QString _userName;
    MyTcpsocket *m_socket;

private slots:
    void on_upload_BT_clicked();
    void on_fileSelect_TB_clicked();
    void on_remarkSelect_TB_clicked();
    void fileNameSlot(QString fileName);
    void remarkNameSlot(QString remarkName);
    void on_remark_CB_clicked();
signals:
    void fileSelectSignal(QString,QString,QString,QString,QString,QString,QString);
    void fileNameSignal(QString);
    void remarkNameSignal(QString);
};

#endif // ADD_COLOR_H
