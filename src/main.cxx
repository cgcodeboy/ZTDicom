/**************************************************************************
*Copyright:Open source
*Author: cgcodeboy
*Date:2017-2-24
*Description:A system integrated with VTK , Qt ,Mysql
**************************************************************************/

#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QCoreApplication>
#include <QFile>

#include <iostream>

vtkStandardNewMacro(ObjPicker)
vtkStandardNewMacro(PolyPicker)
vtkStandardNewMacro(PointPicker)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
