#include "addPackage.h"
#include "ui_addPackage.h"

addPackage::addPackage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addPackage)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
}

addPackage::~addPackage()
{
    delete ui;
}

void addPackage::on_addPackage_BT_clicked()
{
    QString packageName = ui->packageName_LE->text();
    bool doctorRemark = ui->doctorRemark_CB->isChecked();
    emit addPackageSignal(packageName,doctorRemark);
    this->close();
}
