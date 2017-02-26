#ifndef POLY_PICKER_H
#define POLY_PICKER_H

#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkCellPicker.h>
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

class PolyPicker:public vtkInteractorStyleTrackballCamera
{
public:
    static PolyPicker *New();

    PolyPicker()
    {
        selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        selectedActor = vtkSmartPointer<vtkActor>::New();
    }

    virtual void OnLeftButtonDown()
    {
        int *pos = this->GetInteractor()->GetEventPosition();

        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);
        picker->Pick(pos[0],pos[1],pos[2],this->GetDefaultRenderer());

        if(picker->GetCellId() != -1)
        {
            vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(picker->GetCellId());

            vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);

            vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
            selection->AddNode(selectionNode);

            vtkSmartPointer<vtkExtractSelection> extract = vtkSmartPointer<vtkExtractSelection>::New();
            extract->SetInputData(0,polydata);
            extract->SetInputData(1,selection);
            extract->Update();

            selectedMapper->SetInputData((vtkDataSet*)extract->GetOutput());
            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->SetEdgeVisibility(1);
            selectedActor->GetProperty()->SetEdgeColor(0.8,0.5,0.2);
            selectedActor->GetProperty()->SetLineWidth(3);

            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);
        }
        //父类的方法需要执行
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    void setPolydata(vtkPolyData *_polydata)
    {
        this->polydata = _polydata;
    }

private:
    vtkSmartPointer<vtkPolyData> polydata;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
    vtkSmartPointer<vtkActor> selectedActor;
};

#endif
