#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QPixmap>
#include <QPalette>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

#include <iostream>

#include "myTcpsocket.h"

using namespace std;

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0,MyTcpsocket *socket = NULL);
    ~login();

private:
    Ui::login *ui;
    void closeEvent(QCloseEvent *event);
    bool loginStatus;
    MyTcpsocket *m_socket;

signals:
    void loginSignal(QString);
    void closeSignal();
private slots:
    void on_login_BT_clicked();
};

#endif // LOGIN_H
