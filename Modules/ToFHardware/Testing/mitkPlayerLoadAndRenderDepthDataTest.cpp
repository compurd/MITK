/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

//MITK
#include "mitkTestingMacros.h"
#include "mitkRenderingTestHelper.h"
#include "mitkToFCameraMITKPlayerDevice.h"

//VTK
#include <vtkRegressionTestImage.h>

#include <mitkToFConfig.h>
#include <mitkPixelType.h>
#include <itksys/SystemTools.hxx>


int mitkPlayerLoadAndRenderDepthDataTest(int argc, char* argv[])
{
  MITK_TEST_BEGIN("mitkPlayerLoadAndRenderDepthDataTest");

  try
  {
    mitk::ToFCameraMITKPlayerDevice::Pointer playerDevice = mitk::ToFCameraMITKPlayerDevice::New();

    MITK_TEST_CONDITION_REQUIRED(argc >=2, "Testing if enough input parameters are set. Usage: Testname, ImageName (must be in MITK_TOF_DATA_DIR), -V /path/to/reference/screenshot");
    std::string dirname = MITK_TOF_DATA_DIR;
    std::string distanceFileName = dirname + "/" + argv[1];
    playerDevice->SetProperty("DistanceImageFileName",mitk::StringProperty::New(distanceFileName));

    MITK_TEST_CONDITION_REQUIRED(playerDevice->IsCameraActive()==false,"The device (player) should not be active before starting.");
    MITK_TEST_CONDITION_REQUIRED(playerDevice->ConnectCamera()==true,"ConnectCamera() should return true in case of success.");
    MITK_TEST_OUTPUT(<< "Device connected");
    playerDevice->StartCamera();
    MITK_TEST_OUTPUT(<< "Device started");
    MITK_TEST_CONDITION_REQUIRED(playerDevice->IsCameraActive()==true,"After starting the device, the device should be active.");

    //initialize an array with the correct size
    unsigned int captureWidth = playerDevice->GetCaptureWidth();
    unsigned int captureHeight = playerDevice->GetCaptureHeight();
    unsigned int numberOfPixels = captureWidth*captureHeight;
    float* distances = new float[numberOfPixels];
    int imageSequence = 0;

    //fill the array with the device output
    playerDevice->GetDistances(distances,imageSequence);

    //initialize an image and fill it with the array
    unsigned int dimension[2];
    dimension[0] = captureWidth;
    dimension[1] = captureHeight;
    mitk::Image::Pointer mitkDepthImage = mitk::Image::New();
    mitkDepthImage->Initialize(mitk::PixelType(mitk::MakeScalarPixelType<float>()), 2, dimension,1);
    mitkDepthImage->SetSlice(distances);

    //create a node to pass it to the mitkRenderingTestHelper
    mitk::DataNode::Pointer node = mitk::DataNode::New();
    node->SetData(mitkDepthImage);

    // load all arguments into a datastorage, take last argument as reference rendering
    // setup a renderwindow of fixed size X*Y
    // render the datastorage
    // compare rendering to reference image
    mitkRenderingTestHelper renderingHelper(640, 480, argc, argv);
    //Set the opacity for all images
    //for now this test renders in sagittal view direction
    renderingHelper.AddNodeToStorage(node);
    renderingHelper.Render();

    //use this to generate a reference screenshot or save the file:
    bool generateReferenceScreenshot = false;
    if(generateReferenceScreenshot)
    {
      renderingHelper.SaveAsPNG("/home/kilgus/Pictures/output.png");
    }

    //### Usage of vtkRegressionTestImage:
    //vtkRegressionTestImage( vtkRenderWindow )
    //Set a vtkRenderWindow containing the desired scene.
    //vtkRegressionTestImage automatically searches in argc and argv[]
    //for a path a valid image with -V. If the test failed with the
    //first image (foo.png) check if there are images of the form
    //foo_N.png (where N=1,2,3...) and compare against them.
    renderingHelper.PrepareRender();
    int retVal = vtkRegressionTestImage( renderingHelper.GetVtkRenderWindow() );

    //retVal meanings: (see VTK/Rendering/vtkTesting.h)
    //0 = test failed
    //1 = test passed
    //2 = test not run
    //3 = something with vtkInteraction
    MITK_TEST_CONDITION( retVal == 1, "VTK test result positive" );

    //Wait some time to avoid threading issues.
    itksys::SystemTools::Delay(1000);
    playerDevice->StopCamera();
    MITK_TEST_OUTPUT(<< "Device stopped");
    MITK_TEST_CONDITION_REQUIRED(playerDevice->IsCameraActive()==false,"After stopping the device, the device should be inactive.");
    MITK_TEST_CONDITION_REQUIRED(playerDevice->DisconnectCamera()==true, "DisconnectCamera() should return true in case of success.");
    MITK_TEST_OUTPUT(<< "Device disconnected");
    delete[] distances;
  }
  catch(std::exception  &e)
  {
    MITK_ERROR << "Unknown exception occured: " << e.what();
  }

  MITK_TEST_END();
}
