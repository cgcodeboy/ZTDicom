#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent, MyTcpsocket *socket) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    m_socket = socket;
    loginStatus = false;

    ui->image_Widget->setAutoFillBackground(true);
    QPixmap bgpic("./image/glass.bmp");
    QPalette palette;
    palette.setBrush(QPalette::Background,bgpic);
    ui->image_Widget->setPalette(palette);

//    this->setStyleSheet("QWidget{background-color: rgb(170, 170, 127);\
//                                border:1px rgb(0, 170, 255);}\
//                         QWidget#login{border: 1px solid rgb(50, 50, 50);\
//                                background:rgb(170, 170, 255);}\
//                         QPushButton{background-color: rgb(85, 85, 127);}");
//    connect(this,SIGNAL(loginSignal()),this,SLOT(on_login_BT_clicked()));
}

login::~login()
{
    delete ui;
}

void login::closeEvent(QCloseEvent *event)
{
    if(!loginStatus)
        emit closeSignal();
}

void login::on_login_BT_clicked()
{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("localhost");
//    db.setDatabaseName("vtk_pic");
//    db.setUserName("root");
//    db.setPassword("dahuaidan");
//    bool ok = db.open();
//    if(!ok)
//    {
//        QMessageBox::warning(this,"登陆信息","无法连接到数据库！");
//    }
//    else
//    {
//        QSqlQuery query;
//        query.exec(QString("select * from user_table where userName = '%1' and password = '%2'")\
//                   .arg(ui->user_LE->text()).arg(ui->password_LE ->text()));
//        while(query.next())
//        {
//            QString str = query.value(2).toString();
//            cout<<str.toStdString();
//            emit loginSignal(ui->user_LE->text());
//            loginStatus = true;
//            this->hide();
//        }
//    }
    m_socket->writeData(QString("select * from user_table where userName = '%1' and password = '%2'")\
                                           .arg(ui->user_LE->text()).arg(ui->password_LE ->text()));
    m_socket->waitData();
    QString str = m_socket->getData();
    if(str == "ERROR")
    {
        QMessageBox::warning(this,"登录信息","登录信息错误！");
        return;
    }
    else
    {
        emit loginSignal(ui->user_LE->text());
        loginStatus = true;
        this->hide();
    }
}
