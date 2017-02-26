#include "addPiecewise.h"
#include "ui_addPiecewise.h"

addPiecewise::addPiecewise(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addPiecewise)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
}

addPiecewise::~addPiecewise()
{
    delete ui;
}

void addPiecewise::on_addPiecewise_BT_clicked()
{
    double x = ui->x_LCD->value();
    double y = ui->y_LCD->value()/100;
    emit piecewisePoint(x,y);
}
