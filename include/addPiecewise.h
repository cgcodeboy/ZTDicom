#ifndef ADD_PIECEWISE_H
#define ADD_PIECEWISE_H

#include <QWidget>

namespace Ui {
class addPiecewise;
}

class addPiecewise : public QWidget
{
    Q_OBJECT

public:
    explicit addPiecewise(QWidget *parent = 0);
    ~addPiecewise();

private:
    Ui::addPiecewise *ui;

private slots:
    void on_addPiecewise_BT_clicked();

signals:
    void piecewisePoint(double ,double);
};

#endif // ADD_PIECEWISE_H
