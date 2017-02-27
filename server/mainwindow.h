#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QColor>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listen_BT_clicked();

    void on_close_BT_clicked();

    void newConnectSlot();

    void dataComingSlot();

    void disconnectSlot();

    void on_connect_BT_clicked();

    void errorSlot(QAbstractSocket::SocketError error);

private:
    Ui::MainWindow *ui;
    QTcpServer *m_server;
    QMap<QString,QTcpSocket*> *m_socketMap;
    QTcpSocket *currentSocket;
};

#endif // MAINWINDOW_H
