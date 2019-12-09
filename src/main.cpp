#include <vtkActor.h>
#include <vtkVolume.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkWin32OpenGLRenderWindow.h>
#include <vtkRenderWindow.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include <vtkCamera.h>
#include <vtkProperty.h>

#include <vtkStructuredPointsReader.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>

#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>

#include <iostream>
#include <thread>
using namespace std;

vtkSmartPointer<vtkVolume> getVolumeDataActor()
{
    //VTK volume setup
    std::string filePath = "E:\\github\\VTK_test\\build\\bin\\Debug\\ironProt.vtk";
    vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
    reader->SetFileName(filePath.c_str());
    reader->Update();

    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacityTransferFunction->AddPoint(20, 0.0);
    opacityTransferFunction->AddPoint(255, 0.2);

    vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(64.0, 1.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(128.0, 0.0, 0.0, 1.0);
    colorTransferFunction->AddRGBPoint(192.0, 0.0, 1.0, 0.0);
    colorTransferFunction->AddRGBPoint(255.0, 0.0, 0.2, 0.0);

    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();    
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->ShadeOn();
    volumeProperty->SetInterpolationTypeToLinear();

/*     vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New(); */
    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
    volumeMapper->SetBlendModeToComposite();
    volumeMapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
    return volume;
}

vtkSmartPointer<vtkActor> getPolyDataActor()
{
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetResolution(8);
    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());
    vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(0.45, 0.45, 0.45);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);
    return cylinderActor;
}

void KeypressCallbackFunction (
  vtkObject* caller,
  long unsigned int vtkNotUsed(eventId),
  void* vtkNotUsed(clientData),
  void* vtkNotUsed(callData) )
{
  
  vtkRenderWindowInteractor *iren = 
    static_cast<vtkRenderWindowInteractor*>(caller);
  // Close the window
  iren->GetRenderWindow()->Finalize();

  // Stop the interactor
  iren->TerminateApp();
  std::cout << "Closing window..." << std::endl;
}

void RunVTK()
{
    vtkSmartPointer<vtkWin32OpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkWin32OpenGLRenderWindow>::New();
/*     vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New(); */
    renderWindow->OpenGLInitContext();

    vtkSmartPointer<vtkGenericRenderWindowInteractor> Iren = vtkSmartPointer<vtkGenericRenderWindowInteractor>::New();
    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(ren);
    Iren->SetRenderWindow(renderWindow);


    vtkSmartPointer<vtkActor> cylinder = getPolyDataActor();
    ren->AddActor(cylinder);
    ren->SetBackground(0.1, 0.2, 0.4);

/*     vtkSmartPointer<vtkVolume> ironProtVolume = getVolumeDataActor();    
    ren->AddVolume(ironProtVolume);
    ren->SetBackground(1, 1, 1); */

    renderWindow->SetSize(150, 150);

    vtkSmartPointer<vtkCallbackCommand> keypressCallback = vtkSmartPointer<vtkCallbackCommand>::New(); 
    keypressCallback->SetCallback ( KeypressCallbackFunction );
    Iren->AddObserver (vtkCommand::KeyPressEvent, keypressCallback);


    ren->ResetCamera();
    ren->GetActiveCamera()->Zoom(1.5);

/*     renderWindow->OpenGLInitState();
    renderWindow->MakeCurrent(); */

    renderWindow->Render();
    renderWindow->SetWindowName("First Window");

/*     Iren->Initialize(); */
    Iren->Start();

    std::cout << "Window 1 closed..." << std::endl;
    
    // Make another interactor, but use the same render window
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor2 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor2->AddObserver(vtkCommand::KeyPressEvent, keypressCallback);

    renderWindowInteractor2->SetRenderWindow(renderWindow);
    renderWindow->Render();
    renderWindow->SetWindowName("Second Window");

    renderWindowInteractor2->Start();
    std::cout << "Window 2 closed. Exiting..." << std::endl; 
}

int main(int argc, char **argv)
{
    cout << "Program Start!";
    thread t1(RunVTK);    
    t1.join();
    return EXIT_SUCCESS;
}
