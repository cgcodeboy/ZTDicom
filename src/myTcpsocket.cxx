#include "myTcpsocket.h"

MyTcpsocket::MyTcpsocket()
{
    this->str = "";
    this->m_socket = new QTcpSocket;
    m_socket->connectToHost(QHostAddress::LocalHost,1234);
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(disconnectSlot()));
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(m_socket,SIGNAL(QAbstractSocket::SocketError),this,SLOT(errorSlot(QAbstractSocket::SocketError)));
    dataState = false;
}

void MyTcpsocket::writeData(QString message)
{
    qDebug()<<message;
    qDebug()<<this->m_socket->write(message.toLocal8Bit());
}

QString MyTcpsocket::getData()
{
//    dataState = false;
    return this->str;
}

bool MyTcpsocket::getModelState()
{
    return this->dataState;
}

void MyTcpsocket::waitData()
{
    m_socket->waitForReadyRead();
}

void MyTcpsocket::receiveData()
{  
    str = m_socket->readAll();
}

void MyTcpsocket::disconnectSlot()
{
    //    this->m_socket->disconnect();
}

void MyTcpsocket::errorSlot(QAbstractSocket::SocketError error)
{
    qDebug()<<error;
}

//void MyTcpsocket::dataCommingSlot()
//{
//    emit dataComming();
//}
