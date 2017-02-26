#include "addColor.h"
#include "ui_addColor.h"

addColor::addColor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addColor)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
}

addColor::~addColor()
{
    delete ui;
}

void addColor::on_addColor_BT_clicked()
{
    int x = ui->x_LCD->value();
    double r = ui->r_LCD->value()/100;
    double g = ui->g_LCD->value()/100;
    double b = ui->b_LCD->value()/100;
    emit colorPoint(x,r,g,b);
}
