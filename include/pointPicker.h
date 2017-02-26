#ifndef POINT_PICKER_H
#define POINT_PICKER_H

#include <vtkPointPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdTypeArray.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkExtractSelection.h>
#include <vtkRendererCollection.h>
#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>

class PointPicker:public vtkInteractorStyleTrackballCamera
{
public:
    static PointPicker *New();
    vtkTypeMacro(PointPicker, vtkInteractorStyleTrackballCamera)

//    PointPicker()
//    {
//        selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
//        selectedActor = vtkSmartPointer<vtkActor>::New();
//    }

    virtual void OnLeftButtonDown()
    {
        this->Interactor->GetPicker()->Pick(this->GetInteractor()->GetEventPosition()[0],\
                this->GetInteractor()->GetEventPosition()[1],\
                0,this->Interactor->GetRenderWindow()\
                ->GetRenderers()->GetFirstRenderer());

        double pos[3];
        this->Interactor->GetPicker()->GetPickPosition(pos);

        vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->Update();

        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(sphere->GetOutput());

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(1,0,0);
        actor->SetPosition(pos);
        actor->SetScale(0.5);

        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);

        //父类的方法需要执行
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

private:
//    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
//    vtkSmartPointer<vtkActor> selectedActor;
};

#endif
