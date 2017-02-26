#ifndef ADD_COLOR_H
#define ADD_COLOR_H

#include <QWidget>

namespace Ui {
class addColor;
}

class addColor : public QWidget
{
    Q_OBJECT

public:
    explicit addColor(QWidget *parent = 0);
    ~addColor();

private:
    Ui::addColor *ui;

private slots:
    void on_addColor_BT_clicked();

signals:
    void colorPoint(int ,double,double,double);
};

#endif // ADD_COLOR_H
