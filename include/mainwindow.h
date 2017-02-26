#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMutex>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkMetaImageReader.h>
#include <QLinearGradient>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkVolume16Reader.h>
#include <vtkVolumeReader.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionWidget.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation3D.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>


#include "login.h"
#include "addPiecewise.h"
#include "addColor.h"
#include "addPackage.h"
#include "addPatient.h"

#include "volumeClass.h"
#include "objPicker.h"
#include "pointPicker.h"
#include "polyPicker.h"
#include "myTcpsocket.h"
//#include "myThread.h"


class Ui_MainWindow;
class Ui_addPiecewise;
class Ui_addColor;
class Ui_addPackage;
class Ui_addPatient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui_MainWindow *ui;
    login *inWidget;
    VolumeClass *volumeclass;
    int currentVtkwidget;
//    Mythread *m_socketThread;
    MyTcpsocket *m_tcpSocket;
    QString innerData;
    list< vtkSmartPointer< vtkDistanceWidget > > *distanceWidgetList;
    list< vtkSmartPointer< vtkSliderWidget > > *sliderWidgetList;
    list< vtkSmartPointer< vtkCaptionWidget > > *captionWidgetList;

    void updatePackageComboxFun();

private slots:
    void on_actionOpen_DICOM_DIR_triggered();
    void on_actionOpen_VTK_FILE_triggered();
    void on_actionLogin_triggered();
    void on_actionExit_triggered();
    void dirNameSelectedSlot(QString dir);
    void fileNameSelectedSlot(QString fileName);
    void on_addPiecewiseWindow_BT_clicked();
    void on_piecewiseWidget_BT_clicked();
    void on_queryPackaage_BT_clicked();
    void on_colorWidget_BT_clicked();
    void on_addColorWindow_BT_clicked();
    void on_picker_BT_clicked();
    void on_volumeTool_BT_clicked();
    void on_removePiecewisePoint_BT_clicked();
    void on_removeColorPoint_BT_clicked();
    void on_addPackage_BT_clicked();
    void on_objPicker_BT_clicked();
    void on_polyPicker_BT_clicked();
    void on_pointPicker_BT_clicked();
    void on_writeRemark_BT_clicked();
    void on_labelTool_BT_clicked();
    void on_saveRemark_BT_clicked();
    void on_deletePackage_BT_clicked();
    void on_deletePatient_BT_clicked();
    void on_upload_BT_clicked();
    void on_download_BT_clicked();
    void on_sliderTool_BT_clicked();
    void on_distantTool_BT_clicked();
    void addPiecewiseSlot(double x, double y);
    void addColorSlot(int x,double r,double g,double b);
    void addPackageSlot(QString packageName,bool doctorRemark);
    void uploadPatientSlot(QString patientName,QString fileName,QString patientAge,QString patientSex,QString patientIllness,QString packageName,QString remarkName);
    void on_package_TV_clicked(const QModelIndex &index);
    void loginSlot(QString userName);
    void dataCommingSlot(QString str);

signals:
    void dirNameSelected(QString);
    void fileNameSelected(QString);
};

#endif // MAINWINDOW_H
