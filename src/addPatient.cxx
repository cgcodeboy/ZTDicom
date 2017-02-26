#include "addPatient.h"
#include "ui_addPatient.h"

addPatient::addPatient(QWidget *parent,QString userName,MyTcpsocket *socket) :
    QWidget(parent),
    ui(new Ui::addPatient)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    connect(this,SIGNAL(fileNameSignal(QString)),this,SLOT(fileNameSlot(QString)));
    connect(this,SIGNAL(remarkNameSignal(QString)),this,SLOT(remarkNameSlot(QString)));
    this->_userName = userName;

    m_socket = socket;

    this->updatePackageFunc();
}

addPatient::~addPatient()
{
    delete ui;
}

void addPatient::updatePackageFunc()
{
    ui->package_CB->clear();
    int remarkChecked = 0;
    if(ui->remark_CB->isChecked())
    {
        remarkChecked = 1;
    }
//    QSqlQuery query;
//    query.exec(QString("select tableName from link_user_package_table where userName = '%1'").arg(_userName));
//    while(query.next())
//    {
//        bool ok = query.exec(QString("select * from %1 ").arg(query.value(0).toString()));
//        if(ok)
//        {
//            ui->package_CB->clear();
//            while(query.next())
//            {
//                if(query.value(1) == remarkChecked)
//                    ui->package_CB->addItem(query.value(0).toString());
//            }
//        }
//    }
    m_socket->writeData(QString("select tableName from link_user_package_table where userName = '%1'").arg(_userName));
    m_socket->waitData();
    QString str = m_socket->getData();
    qDebug()<<str;
    if(str == "ERROR")
    {
        QMessageBox::warning(this,"添加包信息","添加失败！");
        return;
    }
    else
    {
        str.replace("#","");
        str.replace("@","");
        m_socket->writeData(QString("select * from %1 ").arg(str));
        m_socket->waitData();
        str = m_socket->getData();
        qDebug()<<str;
        if(str == "ERROR")
        {
            QMessageBox::warning(this,"添加包信息","添加失败！");
            return;
        }
        else
        {
            int index_1 = 0;
            while(!str.isEmpty())
            {
                index_1 = str.indexOf('@');
                QString str_1 = str.left(index_1);
                int index_2 =0;
                while(!str_1.isEmpty())
                {
                    index_2 = str_1.indexOf('#');
                    QString cur = str_1.left(index_2);
                    qDebug()<<cur;

                    str_1.remove(0,index_2+1);
                    QString remarkStr = str_1.left(1);
                    qDebug()<<remarkStr;
                    if(remarkStr.toInt() == remarkChecked)
                    {
                        qDebug()<<"in";
                        ui->package_CB->addItem(cur);
                    }
                    str_1.clear();
                }
                str.remove(0,index_1+1);
            }
        }
    }
}

//点击上传病人信息槽函数
void addPatient::on_upload_BT_clicked()
{
    QString patientName = ui->patientName_LE->text();
    QString patientAge = ui->patientAge_LE->text();
    QString patientSex = ui->patientSex_CB->currentText();
    QString packageName = ui->package_CB->currentText();
    QString fileName = ui->fileName_LE->text();
    QString patientIllness = ui->patientIllness_LE->text();
    QString remarkName = ui->remarkName_LE->text();
    if(ui->remark_CB->isChecked())
    {
        if(patientAge.isEmpty()||patientName.isEmpty()||patientSex.isEmpty()||packageName.isEmpty()||fileName.isEmpty()||patientIllness.isEmpty()||remarkName.isEmpty())
        {
            QMessageBox::warning(this,"提示信息","信息未填写完整");
        }
        else
        {
            emit fileSelectSignal(patientName,fileName,patientAge,patientSex,patientIllness,packageName,remarkName);
            this->close();
        }
    }
    else
    {
        if(patientAge.isEmpty()||patientName.isEmpty()||patientSex.isEmpty()||packageName.isEmpty()||fileName.isEmpty()||patientIllness.isEmpty())
        {
            QMessageBox::warning(this,"提示信息","信息未填写完整");
        }
        else
        {
            emit fileSelectSignal(patientName,fileName,patientAge,patientSex,patientIllness,packageName,remarkName);
            this->close();
        }
    }

}

//选择文件槽函数
void addPatient::on_fileSelect_TB_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"上传文件",QDir::currentPath(),tr("DICOM Files (*.vtk *.mhd *.raw)"));
    emit fileNameSignal(fileName);
}

void addPatient::on_remarkSelect_TB_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"上传文件",QDir::currentPath(),tr("医嘱文件 (*.txt)"));
    emit remarkNameSignal(fileName);
}

void addPatient::fileNameSlot(QString fileName)
{
    ui->fileName_LE->setText(fileName);
}

void addPatient::remarkNameSlot(QString remarkName)
{
    ui->remarkName_LE->setText(remarkName);
}

void addPatient::on_remark_CB_clicked()
{
    if(ui->remark_CB->isChecked())
    {
        ui->remarkName_LE->setEnabled(true);
        ui->remarkSelect_TB->setEnabled(true);
        this->updatePackageFunc();
    }
    else
    {
        ui->remarkName_LE->setEnabled(false);
        ui->remarkSelect_TB->setEnabled(false);
        this->updatePackageFunc();
    }
}

