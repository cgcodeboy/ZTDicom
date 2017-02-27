#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->m_server = new QTcpServer;
    this->m_socketMap = new QMap<QString,QTcpSocket*>;
    connect(m_server,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
    m_server->deleteLater();
}

void MainWindow::on_listen_BT_clicked()
{
    if(!m_server->isListening())
    {
        m_server->listen(QHostAddress::Any,1234);
    }
}

void MainWindow::on_close_BT_clicked()
{
    m_server->close();
    QMap<QString,QTcpSocket*>::iterator it = m_socketMap->begin();
    while(it!=m_socketMap->end())
    {
        qDebug()<<it.key();
        it.value()->close();
        it++;
    }
}

void MainWindow::newConnectSlot()
{
    QColor color(50,200,50);
    ui->info_Text->setTextColor(color);
    currentSocket = m_server->nextPendingConnection();
    QString currentAdr = currentSocket->localAddress().toString();
    m_socketMap->insert(currentAdr,currentSocket);
    ui->info_Text->append("新的连接已建立！"+currentAdr);
    connect(currentSocket,SIGNAL(readyRead()),this,SLOT(dataComingSlot()));
    connect(currentSocket,SIGNAL(disconnected()),this,SLOT(disconnectSlot())); 
    connect(currentSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorSlot(QAbstractSocket::SocketError)));
}

void MainWindow::dataComingSlot()
{
    qDebug()<<"data is here";
    if(currentSocket->bytesAvailable())
    {
        QString str = currentSocket->readLine();
        QString client_name = currentSocket->localAddress().toString();
//        QColor color(50,50,200);
//        ui->info_Text->setTextColor(color);
//        ui->info_Text->append(client_name+":  "+ str);
//        QString dataStr;
//        for(int i =0;i<model->rowCount();i++)
//        {
//            for(int j = 0;j<model->columnCount();j++)
//            {
//                dataStr.append(model->index(i,j).data().toString()+"#");
//            }
//            dataStr.append("@");
//        }
//        if(dataStr.isEmpty())
//        {
//            currentSocket->write("NULL");
//            return;
//        }

//        currentSocket->write(dataStr.toLocal8Bit());
        QSqlQuery query;
        bool ok = query.exec(str);
        if(!ok)
        {
            currentSocket->write(QString("ERROR").toLocal8Bit());
            return;
        }
        QString returnData;
        int count = query.record().count();
        while(query.next())
        {
            for(int i = 0;i<count;i++)
            {
                returnData.append(query.value(i).toString()+"#");
            }
            returnData.append("@");
        }
        qDebug()<<returnData;
        if(returnData == "")
        {
            qDebug()<<"it's null";
            currentSocket->write(QString("NULL").toLocal8Bit());
            return;
        }
//        if(returnData.isEmpty())
//        {
//            qDebug()<<"it's empty";
//            currentSocket->write(QString("EMPTY").toLocal8Bit());
//        }
        currentSocket->write(returnData.toLocal8Bit());

        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(query);
        QTableView *view = ui->model_VIEW;
        view->setModel(model);
        view->show();
    }
}

void MainWindow::disconnectSlot()
{
    if(!m_server->isListening())
    {
        QMessageBox::information(this,"连接信息","停止监听，所有连接已断开");
        return;
    }
    QMessageBox::warning(this,"连接信息","有连接已断开！");
}

void MainWindow::on_connect_BT_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName(ui->dbname_LE->text().toStdString().c_str());
    db.setUserName(ui->user_LE->text().toStdString().c_str());
    db.setPassword(ui->password_LE->text().toStdString().c_str());
    bool ok = db.open();
    if(!ok)
    {
        QMessageBox::warning(this,"登陆信息","连接错误或者用户名、密码错误！");
        return;
    }
    QColor color(200,50,50);
    ui->info_Text->setTextColor(color);
    ui->info_Text->append("数据库已连接！");
}

void MainWindow::errorSlot(QAbstractSocket::SocketError error)
{
    qDebug()<<error;
}
