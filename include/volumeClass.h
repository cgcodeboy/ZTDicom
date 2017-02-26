#ifndef VOLUME_CLASS_H
#define VOLUME_CLASS_H

#include <vtkSmartPointer.h>
#include <vtkImageCast.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolume.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>

class VolumeClass{
private:
    vtkSmartPointer<vtkImageCast> _cast;
    vtkSmartPointer<vtkVolume> _volume;
    vtkSmartPointer<vtkPiecewiseFunction> _piecewise;
    vtkSmartPointer<vtkColorTransferFunction> _color;
    vtkSmartPointer<vtkVolumeProperty> _property;
    vtkSmartPointer<vtkVolumeRayCastCompositeFunction> _raycastFunction;
    vtkSmartPointer<vtkVolumeRayCastMapper> _mapper;

public:
    VolumeClass()
    {
        this->_cast = vtkSmartPointer<vtkImageCast>::New();
        this->_volume = vtkSmartPointer<vtkVolume>::New();
        this->_piecewise = vtkSmartPointer<vtkPiecewiseFunction>::New();
        this->_color = vtkSmartPointer<vtkColorTransferFunction>::New();
        this->_property = vtkSmartPointer<vtkVolumeProperty>::New();
        this->_raycastFunction = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
        this->_mapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    }

    //内部变量的set函数
    void setImageCast(vtkSmartPointer<vtkImageCast> imagecast)
    {
        this->_cast = imagecast;
    }
    void setVolume(vtkSmartPointer<vtkVolume> volume)
    {
        this->_volume = volume;
    }
    void setPiecewiseFunction(vtkSmartPointer<vtkPiecewiseFunction> piecewiseFunction)
    {
        this->_piecewise = piecewiseFunction;
    }
    void setColorTransferFunction(vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction)
    {
        this->_color = colorTransferFunction;
    }
    void setVolumeProperty(vtkSmartPointer<vtkVolumeProperty> volumeProperty)
    {
        this->_property = volumeProperty;
    }
    void setVolumeRayCastCompositeFunction(vtkSmartPointer<vtkVolumeRayCastCompositeFunction> volumeRayCastCompositeFunction)
    {
        this->_raycastFunction = volumeRayCastCompositeFunction;
    }
    void setVolumeRayCastMapper(vtkSmartPointer<vtkVolumeRayCastMapper> volumeRayCastMapper)
    {
        this->_mapper = volumeRayCastMapper;
    }

    //内部变量的get函数
    vtkSmartPointer<vtkImageCast> getImagecast()
    {
        return this->_cast;
    }
    vtkSmartPointer<vtkVolume> getVolume()
    {
        return this->_volume;
    }
    vtkSmartPointer<vtkPiecewiseFunction> getPiecewiseFunction()
    {
        return this->_piecewise;
    }
    vtkSmartPointer<vtkColorTransferFunction> getColorTransferFunction()
    {
        return this->_color;
    }
    vtkSmartPointer<vtkVolumeProperty> getVolumeProperty()
    {
        return this->_property;
    }
    vtkSmartPointer<vtkVolumeRayCastCompositeFunction> getVolumeRayCastCompositeFunction()
    {
        return this->_raycastFunction;
    }
    vtkSmartPointer<vtkVolumeRayCastMapper> getVolumeRayCastMapper()
    {
        return this->_mapper;
    }

    void update()
    {

    }
};

#endif
