#ifndef MY_TCPSOCKET_H
#define MY_TCPSOCKET_H

#include <QTcpSocket>
#include <QDebug>
#include <QStandardItemModel>
#include <QHostAddress>
#include <QThread>

class MyTcpsocket:public QObject{
    Q_OBJECT
public:
    explicit MyTcpsocket();

    void writeData(QString message);

    QString getData();

    bool getModelState();

    void waitData();

private slots:
    void disconnectSlot();

    void errorSlot(QAbstractSocket::SocketError error);

//    void dataCommingSlot();

    void receiveData();

signals:
    void dataComming(QString);

private:
    QTcpSocket *m_socket;
    QString str;
    bool dataState;
};

#endif
