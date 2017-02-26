#include "mainwindow.h"
#include "ui_window.h"
#include "ui_addPiecewise.h"
#include "ui_addColor.h"
#include "ui_addPackage.h"
#include "ui_addPatient.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui = new Ui_MainWindow;
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    volumeclass = new VolumeClass;
    //    m_socketThread = new Mythread;
    m_tcpSocket = new MyTcpsocket;
    distanceWidgetList = new list< vtkSmartPointer < vtkDistanceWidget > >;
    sliderWidgetList = new list< vtkSmartPointer< vtkSliderWidget > >;
    captionWidgetList = new list< vtkSmartPointer< vtkCaptionWidget > >;

    //设置工具栏菜单栏的颜色
    ui->mainToolBar->setStyleSheet("QToolBar{background-color: rgb(61, 70, 61);border: rgb(102, 70, 76) 1px;}");
    ui->menuFile->setStyleSheet("QMenu{background-color: rgb(61, 70, 61);}");
    ui->menuBar->setStyleSheet("QMenuBar{background-color: rgb(61, 65, 61);border: rgb(102, 70, 76) 1px;}");
    ui->vtk_TW->setStyleSheet("QTabWidget::pane {border: none;border-top: 3px solid rgb(0, 160, 230);background: rgb(57, 58, 60);}\
                              QTabWidget::tab-bar {\border: none;}\
                              QTabBar::tab { border: none;border-top-left-radius: 4px;border-top-right-radius: 4px;\
                                             color: rgb(175, 175, 175);background: rgb(61, 65, 61, 30);height: 28px;\
                                             min-width: 85px;margin-right: 5px;padding-left: 5px;padding-right: 5px;}\
                              QTabBar::tab:hover {background: rgb(61, 65, 61, 40);}\
                              QTabBar::tab:selected {color: rgb(61, 65, 61);;background: rgb(0, 160, 230);}\
                              QTabWidget#tabWidget::pane {border: 1px solid rgb(45, 45, 45);background: rgb(57, 58, 60);margin-top: -1px;}\
                              QTabBar#tabBar::tab {border: 1px solid rgb(45, 45, 45);border-bottom: none;background: transparent;}\
                              QTabBar#tabBar::tab:hover {color: rgb(61, 65, 61);;}\
                              QTabBar#tabBar::tab:selected {color: rgb(61, 65, 61);;background: rgb(57, 58, 60);}");

                              ui->person_Widget->setAutoFillBackground(true);
            QPixmap bgpic("./image/head.bmp");
    QPalette palette;
    palette.setBrush(QPalette::Background,bgpic);
    ui->person_Widget->setPalette(palette);

    //    QWidget *spacer = new QWidget(this);
    //    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //    ui->mainToolBar->addWidget(spacer);

    //定义登录界面
    inWidget = new login(this,m_tcpSocket);
    inWidget->show();

    //初始化vtkStackWidget
    QVTKWidget *vtkwidget_1 = new QVTKWidget(ui->vtk_TW);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(vtkwidget_1);
    ui->vtk_TW->widget(0)->setLayout(layout);
    currentVtkwidget = 1;

    //为断层表格设置属性
    ui->piecewise_TW->setColumnWidth(0,89);
    ui->piecewise_TW->setColumnWidth(1,89);
    ui->piecewise_TW->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->piecewise_TW->verticalHeader()->setVisible(false);
    ui->piecewise_TW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //为颜色表格设置属性
    ui->color_TW->setColumnWidth(0,44);
    ui->color_TW->setColumnWidth(1,45);
    ui->color_TW->setColumnWidth(2,45);
    ui->color_TW->setColumnWidth(3,45);
    ui->color_TW->resizeColumnsToContents();
    ui->color_TW->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->color_TW->verticalHeader()->setVisible(false);
    ui->color_TW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //为包视图设置属性
    ui->package_TV->verticalHeader()->setVisible(false);
    ui->package_TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(this,SIGNAL(dirNameSelected(QString)),this,SLOT(dirNameSelectedSlot(QString)));
    connect(this,SIGNAL(fileNameSelected(QString)),this,SLOT(fileNameSelectedSlot(QString)));
    connect(inWidget,SIGNAL(closeSignal()),this,SLOT(close()));
    connect(inWidget,SIGNAL(loginSignal(QString)),this,SLOT(loginSlot(QString)));
    connect(m_tcpSocket,SIGNAL(dataComming(QString)),this,SLOT(dataCommingSlot(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sliderWidgetList;
    delete captionWidgetList;
    delete distanceWidgetList;
}

//更新包下选框函数
void MainWindow::updatePackageComboxFun()
{
    ui->package_CB->clear();
    //    QSqlQuery query;
    //    query.exec(QString("select tableName from link_user_package_table where userName = '%1'").arg(ui->user_LB->text()));
    //    while(query.next())
    //    {
    //        bool ok = query.exec(QString("select tableName from %1").arg(query.value(0).toString()));
    //        if(ok)
    //        {
    //            ui->package_CB->clear();
    //            while(query.next())
    //            {
    //                ui->package_CB->addItem(query.value(0).toString());
    //            }
    //        }
    //    }
    this->m_tcpSocket->writeData(QString("select tableName from link_user_package_table where userName = '%1'").arg(ui->user_LB->text()));
    this->m_tcpSocket->waitData();
    QString str = m_tcpSocket->getData();
    str.replace("#","");
    str.replace("@","");
    this->m_tcpSocket->writeData(QString("select tableName from %1").arg(str));
    this->m_tcpSocket->waitData();
    str = m_tcpSocket->getData();
    qDebug()<<str;
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
            ui->package_CB->addItem(cur);
            str_1.remove(0,index_2+1);
        }
        str.remove(0,index_1+1);
    }

}

//打开文件路径的槽函数
void MainWindow::on_actionOpen_DICOM_DIR_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory(this,"DICOM DIR");
    emit dirNameSelected(dirName);
}

//打开文件的槽函数
void MainWindow::on_actionOpen_VTK_FILE_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"VTK FILE");
    emit fileNameSelected(fileName);
}

//人形菜单栏点击弹出登录窗口槽函数
void MainWindow::on_actionLogin_triggered()
{
    inWidget->show();
}

//退出槽函数
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

//选定体渲染路径后的槽函数
void MainWindow::dirNameSelectedSlot(QString dir)
{
    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName(dir.toStdString().c_str());
    reader->Update();

    volumeclass->getImagecast()->SetInputConnection(reader->GetOutputPort());
    volumeclass->getImagecast()->SetOutputScalarTypeToUnsignedChar();
    volumeclass->getImagecast()->Update();

    volumeclass->getVolumeRayCastMapper()->SetVolumeRayCastFunction(volumeclass->getVolumeRayCastCompositeFunction());
    volumeclass->getVolumeRayCastMapper()->SetInputData(volumeclass->getImagecast()->GetOutput());

    //    volumeclass->getPiecewiseFunction()->AddPoint(0,0.0);
    //    volumeclass->getPiecewiseFunction()->AddPoint(255,0.99);

    //    volumeclass->getColorTransferFunction()->AddRGBPoint(0.0,0.0,0.0,0.0);
    //    volumeclass->getColorTransferFunction()->AddRGBPoint(64.0,1.0,0.0,0.0);
    //    volumeclass->getColorTransferFunction()->AddRGBPoint(128.0,0.0,0.0,1.0);
    //    volumeclass->getColorTransferFunction()->AddRGBPoint(192.0,0.0,1.0,0.0);
    //    volumeclass->getColorTransferFunction()->AddRGBPoint(255.0,0.0,0.2,0.0);

    volumeclass->getVolumeProperty()->SetColor(volumeclass->getColorTransferFunction());
    volumeclass->getVolumeProperty()->SetScalarOpacity(volumeclass->getPiecewiseFunction());

    volumeclass->getVolume()->SetMapper(volumeclass->getVolumeRayCastMapper());
    volumeclass->getVolume()->SetProperty(volumeclass->getVolumeProperty());

    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->SetBackground(0.13, 0.11, 0.17);

    render->AddVolume(volumeclass->getVolume());
    QVTKWidget *widget = ui->vtk_TW->widget(0)->findChild<QVTKWidget*>();
    widget->GetRenderWindow()->AddRenderer(render);


    //------------Imageviewer2D---------------------------
    vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputData(reader->GetOutput());
    viewer->SetRenderWindow(ui->attachVtkwindow->GetRenderWindow());

    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    ui->attachVtkwindow->GetInteractor()->SetInteractorStyle(style);
    viewer->Render();
}

//选定文件后的槽函数
void MainWindow::fileNameSelectedSlot(QString fileName)
{
    int index = fileName.lastIndexOf("/");
    QString file = fileName.right(fileName.length()-index-1);
    QVTKWidget *vtkwidget = new QVTKWidget(ui->vtk_TW);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(vtkwidget);
    currentVtkwidget++;
    if(currentVtkwidget>2)
    {
        QWidget *widget = new QWidget;
        ui->vtk_TW->addTab(widget,"#"+QString::number(currentVtkwidget)+"渲染界面 "+file);
        ui->vtk_TW->widget(currentVtkwidget-1)->setLayout(layout);
    }
    else
    {
        ui->vtk_TW->setTabText(currentVtkwidget-1,"#"+QString::number(currentVtkwidget)+"渲染界面 "+file);
        ui->vtk_TW->widget(currentVtkwidget-1)->setLayout(layout);
    }
    cout<<currentVtkwidget<<" "<<ui->vtk_TW->count()<<endl;
    //    if(currentVtkwidget>2)
    //    {
    //        QWidget *widget = new QWidget;
    //        ui->vtk_TW->addTab(widget,QString::number(currentVtkwidget));
    //    }

    index = fileName.indexOf(".");
    QString suffix = fileName.right(fileName.length()-index-1);
    if(suffix == "mhd")
    {
        vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
        reader->SetFileName(fileName.toStdString().c_str());
        reader->Update();

        vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
        viewer->SetInputConnection(reader->GetOutputPort());
        viewer->SetRenderWindow(vtkwidget->GetRenderWindow());
        viewer->SetColorLevel(500);
        viewer->SetColorWindow(2000);
        viewer->SetSlice(40);
        viewer->SetSliceOrientationToXY();
        viewer->Render();
    }
    else if(suffix == "vtk")
    {
        vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
        reader->SetFileName(fileName.toStdString().c_str());
        reader->Update();

        VolumeClass *newVolume = new VolumeClass;

        newVolume->getVolumeRayCastMapper()->SetInputConnection(reader->GetOutputPort());
        newVolume->getVolumeRayCastMapper()->SetVolumeRayCastFunction(newVolume->getVolumeRayCastCompositeFunction());

        newVolume->getPiecewiseFunction()->AddPoint(70,   0.00);
        newVolume->getPiecewiseFunction()->AddPoint(90,   0.40);
        newVolume->getPiecewiseFunction()->AddPoint(180,  0.60);

        newVolume->getColorTransferFunction()->AddRGBPoint(0.000,  0.00, 0.00, 0.00);
        newVolume->getColorTransferFunction()->AddRGBPoint(64.00,  1.00, 0.52, 0.30);
        newVolume->getColorTransferFunction()->AddRGBPoint(190.0,  1.00, 1.00, 1.00);
        newVolume->getColorTransferFunction()->AddRGBPoint(220.0,  0.20, 0.20, 0.20);

        newVolume->getVolumeProperty()->SetInterpolationTypeToLinear();
        newVolume->getVolumeProperty()->SetAmbient(0.4);
        newVolume->getVolumeProperty()->SetDiffuse(0.6);
        newVolume->getVolumeProperty()->SetSpecular(0.2);
        newVolume->getVolumeProperty()->SetScalarOpacity(newVolume->getPiecewiseFunction());
        newVolume->getVolumeProperty()->SetColor(newVolume->getColorTransferFunction());

        newVolume->getVolume()->SetMapper(newVolume->getVolumeRayCastMapper());
        newVolume->getVolume()->SetProperty(newVolume->getVolumeProperty());

        vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
        render->SetBackground(1.0,1.0,1.0);
        render->AddVolume(newVolume->getVolume());

        vtkwidget->GetRenderWindow()->AddRenderer(render);
    }
    else
    {

    }
}

//点击弹出添加断层点窗口槽函数
void MainWindow::on_addPiecewiseWindow_BT_clicked()
{
    addPiecewise *piecewise = new addPiecewise(this);
    piecewise->show();
    connect(piecewise,SIGNAL(piecewisePoint(double,double)),this,SLOT(addPiecewiseSlot(double,double)));
}

//点击stackwidget跳转到断层点窗口槽函数
void MainWindow::on_piecewiseWidget_BT_clicked()
{
    ui->tool_SW->setCurrentIndex(0);
}

//点击查看包内容槽函数
void MainWindow::on_queryPackaage_BT_clicked()
{
    QString packageName = ui->package_CB->currentText();

    QStandardItemModel *model = new QStandardItemModel;
    //    model->setQuery(QString("select * from %1").arg(packageName));
    m_tcpSocket->writeData(QString("select * from %1").arg(packageName));
    int columnCount = 0;
    int rowCount = 0;
    m_tcpSocket->waitData();
    QString str = m_tcpSocket->getData();
    if(str == "NULL")
    {
        model->clear();
        ui->package_TV->setModel(model);
        ui->package_TV->show();
        return;
    }
    else if(str == "ERROR")
    {
        QMessageBox::warning(this,"查询信息","查询出错");
        return;
    }

    model->clear();

    qDebug()<<"str"<<str;

    QString str2(str);
    int index_1 = 0;

    while(!str.isEmpty())
    {
        index_1 = str.indexOf('@');
        QString str_1 = str.left(index_1);
        int index_2 =0;
        while(!str_1.isEmpty())
        {
            index_2 = str_1.indexOf('#');
            str_1.remove(0,index_2+1);
            columnCount++;
        }
        str.remove(0,index_1+1);
        rowCount++;
    }
    qDebug()<<"column:"<<columnCount<<" "<<rowCount;
    columnCount = columnCount/rowCount;
    model->setColumnCount(columnCount);
    model->setRowCount(rowCount);

    index_1 = 0;
    int i = 0,j = 0;
    while(!str2.isEmpty())
    {
        index_1 = str2.indexOf('@');
        QString str_1 = str2.left(index_1);
        int index_2 =0;
        while(!str_1.isEmpty())
        {
            index_2 = str_1.indexOf('#');
            QString cur = str_1.left(index_2);
            qDebug()<<cur;
            QStandardItem *itemID = new QStandardItem(cur);
            model->setItem(i,j,itemID);
            str_1.remove(0,index_2+1);
            j++;
        }
        j = 0;
        str2.remove(0,index_1+1);
        i++;
    }

    //    QStandardItemModel *model;
    //    while(model == NULL)
    //    {
    //            if(m_tcpSocket->getModelState())
    //            {
    //model = m_tcpSocket->getModel();
    //                model->setHeaderData(0,Qt::Horizontal,"姓名");
    //                model->setHeaderData(1,Qt::Horizontal,"文件名");
    //                model->setHeaderData(2,Qt::Horizontal,"年龄");
    //                model->setHeaderData(3,Qt::Horizontal,"性别");
    //                model->setHeaderData(4,Qt::Horizontal,"病症");
    //            }
    //}
    //ui->package_TV->setModel(model);
    //    m_socketThread->writeData(QString("select * from %1").arg(packageName));
    //    QStandardItemModel *model;
    //    m_socketThread->run();
    //    while(model == NULL)
    //    {
    //        qDebug()<<"it's null";
    //        if(m_socketThread->getSocketModelState())
    //        {
    //            model = m_socketThread->getModel();
    //        }
    //    }
    ui->package_TV->setModel(model);
    ui->package_TV->show();
}

//点击stackwidget跳转到颜色点窗口槽函数
void MainWindow::on_colorWidget_BT_clicked()
{
    ui->tool_SW->setCurrentIndex(1);
}

//点击弹出添加RGB颜色点窗口槽函数
void MainWindow::on_addColorWindow_BT_clicked()
{
    addColor *addcolor = new addColor(this);
    addcolor->show();
    connect(addcolor,SIGNAL(colorPoint(int,double,double,double)),this,SLOT(addColorSlot(int,double,double,double)));
}

//点选按钮点击槽函数
void MainWindow::on_picker_BT_clicked()
{
    ui->tool_SW->setCurrentIndex(2);
}

//点击stackwidget跳转到小工具窗口槽函数
void MainWindow::on_volumeTool_BT_clicked()
{
    ui->tool_SW->setCurrentIndex(3);
}

//点击移除选中的断层点槽函数
void MainWindow::on_removePiecewisePoint_BT_clicked()
{
    int row = ui->piecewise_TW->currentRow();

    QString pointX = ui->piecewise_TW->item(row,0)->text();
    volumeclass->getPiecewiseFunction()->RemovePoint(pointX.toInt());
    QVTKWidget *widget = ui->vtk_TW->widget(0)->findChild<QVTKWidget*>();
    widget->update();

    ui->piecewise_TW->removeRow(row);
}

//点击移除选中的颜色点槽函数
void MainWindow::on_removeColorPoint_BT_clicked()
{
    int row = ui->color_TW->currentRow();

    QString pointX = ui->color_TW->item(row,0)->text();
    volumeclass->getColorTransferFunction()->RemovePoint(pointX.toDouble());
    QVTKWidget *widget = ui->vtk_TW->widget(0)->findChild<QVTKWidget*>();
    widget->update();

    ui->color_TW->removeRow(row);
}

//添加包槽函数
void MainWindow::on_addPackage_BT_clicked()
{
    addPackage *addpackage = new addPackage(this);
    addpackage->show();
    connect(addpackage,SIGNAL(addPackageSignal(QString,bool)),this,SLOT(addPackageSlot(QString,bool)));
}

//点击点选物体按钮槽函数
void MainWindow::on_objPicker_BT_clicked()
{
    vtkSmartPointer<ObjPicker> objPickerStyle = vtkSmartPointer<ObjPicker>::New();
    QVTKWidget *widget = ui->vtk_TW->currentWidget()->findChild<QVTKWidget*>();
    objPickerStyle->SetDefaultRenderer(widget->GetRenderWindow()\
                                       ->GetRenderers()->GetFirstRenderer());
    widget->GetInteractor()->SetInteractorStyle(objPickerStyle);
}

//点击点选多边形按钮槽函数
void MainWindow::on_polyPicker_BT_clicked()
{
    //未实现
    vtkSmartPointer<PolyPicker> polyPickerStyle = vtkSmartPointer<PolyPicker>::New();
    QVTKWidget *widget = ui->vtk_TW->currentWidget()->findChild<QVTKWidget*>();
    polyPickerStyle->SetDefaultRenderer(widget->GetRenderWindow()\
                                        ->GetRenderers()->GetFirstRenderer());
    widget->GetInteractor()->SetInteractorStyle(polyPickerStyle);
}

//点击点选点按钮槽函数
void MainWindow::on_pointPicker_BT_clicked()
{
    vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
    vtkSmartPointer<PointPicker> pointPickerStyle = vtkSmartPointer<PointPicker>::New();
    QVTKWidget *widget = ui->vtk_TW->currentWidget()->findChild<QVTKWidget*>();
    pointPickerStyle->SetDefaultRenderer(widget->GetRenderWindow()\
                                         ->GetRenderers()->GetFirstRenderer());
    widget->GetInteractor()->SetPicker(picker);
    widget->GetInteractor()->SetInteractorStyle(pointPickerStyle);
}

//编辑医嘱按钮槽函数
void MainWindow::on_writeRemark_BT_clicked()
{
    ui->remark_PT->setReadOnly(false);
}

//添加标签按钮点击槽函数
void MainWindow::on_labelTool_BT_clicked()
{
    QVTKWidget *widget = ui->vtk_TW->currentWidget()->findChild<QVTKWidget*>();
    vtkSmartPointer<vtkCaptionRepresentation> representation = vtkSmartPointer<vtkCaptionRepresentation>::New();
    representation->GetCaptionActor2D()->SetCaption("Test caption");
    representation->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(100);
    double pos[] = {0.5,0,0};
    representation->SetAnchorPosition(pos);

    vtkSmartPointer<vtkCaptionWidget> captionWidget = vtkSmartPointer<vtkCaptionWidget>::New();
    captionWidgetList->push_back(captionWidget);
    captionWidget->SetInteractor(widget->GetInteractor());
    captionWidget->SetRepresentation(representation);
    captionWidget->On();

    widget->GetRenderWindow()->Render();
}

//保存医嘱按钮槽函数
void MainWindow::on_saveRemark_BT_clicked()
{
    ui->remark_PT->setReadOnly(true);
}

//点击删除包槽函数
void MainWindow::on_deletePackage_BT_clicked()
{
    QMessageBox box;
    box.setText("删除包信息");
    box.setInformativeText("您想删除此包吗？包中数据也将丢失！");
    box.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    int state = box.exec();
    switch (state) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
    {
        //        QSqlQuery query;
        //        bool ok = query.exec(QString("drop table %1").arg(ui->package_CB->currentText()));
        this->m_tcpSocket->writeData(QString("drop table %1").arg(ui->package_CB->currentText()));
        this->m_tcpSocket->waitData();
        QString str = m_tcpSocket->getData();
        if(str == "NULL")
        {
            m_tcpSocket->writeData(QString("select tableName from link_user_package_table where userName = '%1'").arg(ui->user_LB->text()));
            m_tcpSocket->waitData();
            str = m_tcpSocket->getData();
//            if(str == "NULL")
//            {
//                QMessageBox::information(this,"删包信息","删除成功！");
//                this->updatePackageComboxFun();
//            }
            if(str == "ERROR")
            {
                QMessageBox::information(this,"删包信息","出错！");
                return;
            }
            else
            {
                str.replace("#","");
                str.replace("@","");
                m_tcpSocket->writeData(QString("delete from %1 where tableName = '%2' ").arg(str).arg(ui->package_CB->currentText()));
                m_tcpSocket->waitData();
                str = m_tcpSocket->getData();
                if(str == "NULL")
                {
                    QMessageBox::information(this,"删包信息","删除成功！");
                    this->updatePackageComboxFun();
                    return;
                }
                else if(str == "ERROR")
                {
                    QMessageBox::information(this,"删包信息","出错！");
                    return;
                }
            }
        }
        else if(str == "ERROR")
        {
            QMessageBox::information(this,"删包信息","出错！");
            return;
        }

        //        if(ok)
        //        {
        //            query.exec();
        //            while(query.next())
        //            {
        //                ok = query.exec(QString("delete from %1 where tableName = '%2' ").arg(query.value(0).toString()).arg(ui->package_CB->currentText()));
        //                if(ok)
        //                {
        //                    QMessageBox::information(this,"删包信息","删除成功！");
        //                    this->updatePackageComboxFun();
        //                }
        //            }
        //        }
        //        else
        //        {
        //            QMessageBox::information(this,"删包信息","删除失败！");
        //        }
        //        break;
    }
    default:
        break;
    }
}

//删除病人槽函数
void MainWindow::on_deletePatient_BT_clicked()
{
    QMessageBox box;
    box.setText("删除病人信息");
    box.setInformativeText("您想移除病人吗？病人信息也将丢失！");
    box.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    int state = box.exec();
    switch (state) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
    {
//        QSqlQuery query;
//        bool ok = query.exec(QString("delete from %1 where fileName = '%2'").arg(ui->package_CB->currentText()).arg(ui->fileName_LB->text()));
//        if(ok)
        m_tcpSocket->writeData(QString("delete from %1 where fileName = '%2'").arg(ui->package_CB->currentText()).arg(ui->fileName_LB->text()));
        m_tcpSocket->waitData();
        QString str = m_tcpSocket->getData();
        if(str == "NULL")
        {
            QMessageBox::information(this,"删除病人信息","删除成功！");
            this->on_queryPackaage_BT_clicked();
            ui->fileName_LB->clear();
            ui->patientAge_LB->clear();
            ui->patientIllness_LB->clear();
            ui->patientSex_LB->clear();
            ui->patientName_LB->clear();
            //            ui->fileName_LB->clear();
        }
        else if(str == "ERROR")
        {
            QMessageBox::information(this,"删除病人信息","删除失败！");
        }
        break;
    }
    default:
        break;
    }
}

//上传槽函数
void MainWindow::on_upload_BT_clicked()
{
    addPatient *addpatient = new addPatient(this,ui->user_LB->text(),m_tcpSocket);
    addpatient->show();
    connect(addpatient,SIGNAL(fileSelectSignal(QString,QString,QString,QString,QString,QString,QString)),this,SLOT(uploadPatientSlot(QString,QString,QString,QString,QString,QString,QString)));
}

//下载槽函数
void MainWindow::on_download_BT_clicked()
{
    QString downloadDir = QFileDialog::getSaveFileName(this,"存放路径",QDir::currentPath());
}

//滑条按钮点击槽函数
void MainWindow::on_sliderTool_BT_clicked()
{
    QVTKWidget *widget = ui->vtk_TW->currentWidget()->findChild<QVTKWidget*>();
    vtkSmartPointer<vtkSliderRepresentation3D> sliderRep = vtkSmartPointer<vtkSliderRepresentation3D>::New();
    sliderRep->SetMinimumValue(3.0);
    sliderRep->SetMaximumValue(50.0);
    sliderRep->SetValue(10);
    sliderRep->SetTitleText("Sphere Resolution");
    sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToWorld();
    sliderRep->GetPoint1Coordinate()->SetValue(-4,6,0);
    sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToWorld();
    sliderRep->GetPoint2Coordinate()->SetValue(4,6,0);
    sliderRep->SetSliderLength(0.075);
    sliderRep->SetSliderWidth(0.05);
    sliderRep->SetEndCapLength(0.05);

    vtkSmartPointer<vtkSliderWidget> sliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
    sliderWidgetList->push_back(sliderWidget);
    sliderWidget->SetInteractor(widget->GetInteractor());
    sliderWidget->SetRepresentation(sliderRep);
    sliderWidget->SetAnimationModeToAnimate();
    sliderWidget->EnabledOn();
    sliderWidget->Render();
    widget->GetRenderWindow()->Render();
}

void MainWindow::on_distantTool_BT_clicked()
{
    QVTKWidget *widget = ui->vtk_TW->currentWidget()->findChild<QVTKWidget*>();
    vtkSmartPointer<vtkDistanceWidget> distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
    this->distanceWidgetList->push_back(distanceWidget);
    distanceWidget->SetInteractor(widget->GetInteractor());
    distanceWidget->CreateDefaultRepresentation();
    static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm");
    distanceWidget->On();
    widget->GetRenderWindow()->Render();
}

//添加断层点槽函数
void MainWindow::addPiecewiseSlot(double x, double y)
{
    volumeclass->getPiecewiseFunction()->AddPoint(x,y);
    QVTKWidget *widget = ui->vtk_TW->widget(0)->findChild<QVTKWidget*>();
    widget->update();

    int rowCount = ui->piecewise_TW->rowCount();
    ui->piecewise_TW->insertRow(rowCount);
    ui->piecewise_TW->setItem(rowCount,0,new QTableWidgetItem(QString::number(x)));
    ui->piecewise_TW->setItem(rowCount,1,new QTableWidgetItem(QString::number(y)));
}

//添加RGB颜色点槽函数
void MainWindow::addColorSlot(int x, double r, double g, double b)
{
    volumeclass->getColorTransferFunction()->AddRGBPoint(x,r,g,b);
    QVTKWidget *widget = ui->vtk_TW->widget(0)->findChild<QVTKWidget*>();
    widget->update();

    int rowCount = ui->color_TW->rowCount();
    ui->color_TW->insertRow(rowCount);
    ui->color_TW->setItem(rowCount,0,new QTableWidgetItem(QString::number(x)));
    ui->color_TW->setItem(rowCount,1,new QTableWidgetItem(QString::number(r)));
    ui->color_TW->setItem(rowCount,2,new QTableWidgetItem(QString::number(g)));
    ui->color_TW->setItem(rowCount,3,new QTableWidgetItem(QString::number(b)));
}

//添加医嘱槽函数
void MainWindow::addPackageSlot(QString packageName, bool doctorRemark)
{
    cout<<doctorRemark<<packageName.toStdString()<<endl;
    if(doctorRemark)
    {
//        bool ok = query.exec(QString("create table %1 (patientName varchar(20) not null primary key,\
//                                     fileName varchar(20) not null,patientAge int(2) not null ,\
//                                     patientSex int(1) not null,patientIllness varchar(50) not null)").arg(packageName));
        m_tcpSocket->writeData(QString("create table %1 (patientName varchar(20) not null primary key,\
                                                                            fileName varchar(20) not null,patientAge int(2) not null ,\
                                                                            patientSex int(1) not null,patientIllness varchar(50) not null)").arg(packageName));
        m_tcpSocket->waitData();
        QString str = m_tcpSocket->getData();
        if(str == "ERROR")
        {
               QMessageBox::information(this,"建包信息","创建失败！");
        }
         else
        {
            m_tcpSocket->writeData(QString("select tableName from link_user_package_table where userName = '%1'").arg(ui->user_LB->text()));
            m_tcpSocket->waitData();
            str = m_tcpSocket->getData();
            if(str == "ERROR")
            {
                QMessageBox::information(this,"建包信息","创建失败！");
                return;
            }
             else
             {
                str.replace("#","");
                str.replace("@","");
                m_tcpSocket->writeData(QString("insert into %1 values('%2',1)").arg(str).arg(packageName));
                m_tcpSocket->waitData();
                str = m_tcpSocket->getData();
//                 ok = query.exec(QString("insert into %1 values('%2',1)").arg(query.value(0).toString()).arg(packageName));
                 if(str == "NULL")
                 {
                     cout<<"in 2"<<endl;
                     QMessageBox::information(this,"建包信息","创建成功！");
                     this->updatePackageComboxFun();
                 }
                 else if(str == "ERROR")
                 {
                    QMessageBox::information(this,"建包信息","创建失败！");
                    return;
                 }
             }
         }
    }
    else
    {
//        bool ok = query.exec(QString("create table %1 (fileName varchar(20) not null,path varchar(50) not null)").arg(packageName));
//        if(!ok)
//        {
//            QMessageBox::information(this,"建包信息","创建失败！");
//        }
//        else
//        {
//            query.exec(QString("select tableName from link_user_package_table where userName = '%1'").arg(ui->user_LB->text()));
//            while(query.next())
//            {
//                ok = query.exec(QString("insert into %1 values('%2',0)").arg(query.value(0).toString()).arg(packageName));
//                if(ok)
//                {
//                    cout<<"in 2"<<endl;
//                    QMessageBox::information(this,"建包信息","创建成功！");
//                    this->updatePackageComboxFun();
//                }
//            }
//        }
        m_tcpSocket->writeData(QString("create table %1 (patientName varchar(20) not null primary key,\
                                       fileName varchar(20) not null,patientAge int(2) not null ,\
                                       patientSex int(1) not null,patientIllness varchar(50) not null)").arg(packageName));
        m_tcpSocket->waitData();
        QString str = m_tcpSocket->getData();
        if(str == "ERROR")
        {
               QMessageBox::information(this,"建包信息","创建失败！");
        }
         else
        {
            m_tcpSocket->writeData(QString("select tableName from link_user_package_table where userName = '%1'").arg(ui->user_LB->text()));
            m_tcpSocket->waitData();
            str = m_tcpSocket->getData();
            if(str == "ERROR")
            {
                QMessageBox::information(this,"建包信息","创建失败！");
                return;
            }
             else
             {
                str.replace("#","");
                str.replace("@","");
                m_tcpSocket->writeData(QString("insert into %1 values('%2',0)").arg(str).arg(packageName));
                m_tcpSocket->waitData();
                str = m_tcpSocket->getData();
//                 ok = query.exec(QString("insert into %1 values('%2',1)").arg(query.value(0).toString()).arg(packageName));
                 if(str == "NULL")
                 {
                     cout<<"in 2"<<endl;
                     QMessageBox::information(this,"建包信息","创建成功！");
                     this->updatePackageComboxFun();
                 }
                 else if(str == "ERROR")
                 {
                    QMessageBox::information(this,"建包信息","创建失败！");
                    return;
                 }
             }
         }
    }
}

//上传信息填写完整后的槽函数
void MainWindow::uploadPatientSlot(QString patientName, QString fileName, QString patientAge, QString patientSex, QString patientIllness, QString packageName, QString remarkName)
{
    const char* str = patientAge.toLatin1().data();
    while(*str&&*str>='0'&&*str<='9')str++;
    if(*str)
    {
        QMessageBox::warning(this,"提示信息","年龄输入不对！");
        return;
    }
    int patient_age = patientAge.toInt();
    int patient_sex = 0;
    if(patientSex == "女")
    {
        patient_sex = 1;
    }
    QFile file(fileName);
    if(!file.exists())
    {
        QMessageBox::warning(this,"提示信息","上传文件不存在！");
        return;
    }
    QFile remark(remarkName);
    if(!remark.exists()&&remarkName!="")
    {
        QMessageBox::warning(this,"提示信息","上传医嘱不存在！");
        return;
    }

    int index = fileName.lastIndexOf("/");
    QString file_name = fileName.right(fileName.length()-index-1);
//    QSqlQuery query;
    m_tcpSocket->writeData(QString("insert into %1 values('%2','%3',%4,%5,'%6')")\
                           .arg(packageName).arg(patientName).arg(file_name).arg(patient_age).arg(patient_sex).arg(patientIllness));
//    bool ok = query.exec(QString("insert into %1 values('%2','%3',%4,%5,'%6')")\
                         .arg(packageName).arg(patientName).arg(file_name).arg(patient_age).arg(patient_sex).arg(patientIllness));
    m_tcpSocket->waitData();
    QString returnStr = m_tcpSocket->getData();
    if(returnStr == "NULL")
    {
        QMessageBox::information(this,"提示信息","上传成功");
        if(ui->package_CB->currentText() == packageName)
        {
            this->on_queryPackaage_BT_clicked();
        }
    }
    else if(returnStr == "ERROR")
    {
        QMessageBox::information(this,"提示信息","上传失败");
    }
}

//登录后显示个人信息槽函数
void MainWindow::loginSlot(QString userName)
{
//    QSqlQuery query;
//    query.exec(QString("select * from user_table where userName = '%1'").arg(userName));
//    while(query.next())
//    {
//        ui->user_LB->setText(query.value(2).toString());
//        ui->job_LB->setText(query.value(3).toString());
//        ui->part_LB->setText(query.value(4).toString());
//        this->updatePackageComboxFun();

        //        query.exec(QString("select tableName from link_user_package_table where userName = '%1'").arg(userName));
        //        while(query.next())
        //        {
        //            cout<<"in 1"<<endl;
        //            query.exec(QString("select tableName from %1").arg(query.value(0).toString()));
        //            while(query.next())
        //            {
        //                cout<<"in 2"<<endl;
        //                ui->package_CB->addItem(query.value(0).toString());
        //            }
        //        }
//    }
    m_tcpSocket->writeData(QString("select Name,job,part from user_table where userName = '%1'").arg(userName));
    m_tcpSocket->waitData();
    QString str = m_tcpSocket->getData();
    if(str == "ERROR")
    {
        QMessageBox::warning(this,"初始化信息","初始化登录信息失败！");
        return;
    }
    qDebug()<<str;
    int index = 0;
    index  = str.indexOf('#');
    ui->user_LB->setText(str.left(index));
    str.remove(0,index+1);
    index = str.indexOf('#');
    ui->job_LB->setText(str.left(index));
    str.remove(0,index+1);
    index = str.indexOf('#');
    ui->part_LB->setText(str.left(index));
//    m_tcpSocket->writeData(QString("select tableName from link_user_package_table where userName = '%1'").arg(userName));
    this->updatePackageComboxFun();
}

void MainWindow::dataCommingSlot(QString str)
{
    qDebug()<<str;
    this->innerData = str;
}

//包中某项文件被点击槽函数
void MainWindow::on_package_TV_clicked(const QModelIndex &index)
{
    QAbstractItemModel *model = ui->package_TV->model();
    int row = index.row();
    ui->fileName_LB->setText(model->data(model->index(row,1)).toString());
    ui->patientName_LB->setText(model->data(model->index(row,0)).toString());
    ui->patientAge_LB->setText(model->data(model->index(row,2)).toString());
    if(model->data(model->index(row,3)).toString().toInt() == 0)
    {
        ui->patientSex_LB->setText("男");
    }
    else
    {
        ui->patientSex_LB->setText("女");
    }
    ui->patientIllness_LB->setText(model->data(model->index(row,4)).toString());
}
