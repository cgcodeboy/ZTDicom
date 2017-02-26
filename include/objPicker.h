#ifndef OBJ_PICKER_H
#define OBJ_PICKER_H

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>

class ObjPicker:public vtkInteractorStyleTrackballCamera
{
public:
    static ObjPicker *New();

    ObjPicker()
    {
        lastPickedActor = NULL;
        lastPickedProperty = vtkSmartPointer<vtkProperty>::New();
    }

    virtual void OnLeftButtonDown()
    {
        int *clickPos = this->GetInteractor()->GetEventPosition();
        vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
        picker->Pick(clickPos[0],clickPos[1],0,this->GetDefaultRenderer());
        double *pos = picker->GetPickPosition();
        if(this->lastPickedActor)
        {
            this->lastPickedActor->GetProperty()->DeepCopy(this->lastPickedProperty);
        }
        this->lastPickedActor = picker->GetActor();
        if(this->lastPickedActor)
        {
            this->lastPickedProperty->DeepCopy(this->lastPickedActor->GetProperty());
            this->lastPickedActor->GetProperty()->SetColor(0.8,0.7,0.6);
            this->lastPickedActor->GetProperty()->SetDiffuse(1.0);
            this->lastPickedActor->GetProperty()->SetSpecular(0.0);
        }
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
private:
    vtkSmartPointer<vtkActor> lastPickedActor;
    vtkSmartPointer<vtkProperty> lastPickedProperty;
};

#endif
