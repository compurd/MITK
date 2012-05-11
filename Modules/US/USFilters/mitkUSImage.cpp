/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mitkUSImage.h"
#include <mitkStringProperty.h>
#include <mitkProperties.h>


mitk::USImage::USImage() : mitk::Image()
{
  this->SetMetadata(mitk::USImageMetadata::New());
}

mitk::USImage::USImage(mitk::Image::Pointer image) : mitk::Image()
{
  this->Initialize(image);
  this->SetVolume(image->GetData());
  this->SetMetadata(mitk::USImageMetadata::New());
}

mitk::USImage::~USImage()
{
 
}

mitk::USImageMetadata::Pointer mitk::USImage::GetMetadata(){
  mitk::USImageMetadata::Pointer result = mitk::USImageMetadata::New();
 
  result->SetDeviceManufacturer(this->GetProperty(mitk::USImageMetadata::PROP_DEV_MANUFACTURER)->GetValueAsString());
  result->SetDeviceModel(       this->GetProperty(mitk::USImageMetadata::PROP_DEV_MODEL)->GetValueAsString());
  result->SetDeviceComment(     this->GetProperty(mitk::USImageMetadata::PROP_DEV_COMMENT)->GetValueAsString());
  result->SetDeviceIsVideoOnly( this->GetProperty(mitk::USImageMetadata::PROP_DEV_ISVIDEOONLY));
  result->SetProbeName(         this->GetProperty(mitk::USImageMetadata::PROP_PROBE_NAME)->GetValueAsString());
  result->SetProbeFrequency(    this->GetProperty(mitk::USImageMetadata::PROP_PROBE_FREQUENCY)->GetValueAsString());
  result->SetZoom(              this->GetProperty(mitk::USImageMetadata::PROP_ZOOM)->GetValueAsString());
  
  return result;
}


void mitk::USImage::SetMetadata(mitk::USImageMetadata::Pointer metadata){
  this->SetProperty(mitk::USImageMetadata::PROP_DEV_MANUFACTURER, mitk::StringProperty::New(metadata->GetDeviceManufacturer())); 
  this->SetProperty(mitk::USImageMetadata::PROP_DEV_MODEL, mitk::StringProperty::New(metadata->GetDeviceModel())); 
  this->SetProperty(mitk::USImageMetadata::PROP_DEV_COMMENT, mitk::StringProperty::New(metadata->GetDeviceComment()));
  this->SetProperty(mitk::USImageMetadata::PROP_DEV_ISVIDEOONLY, mitk::BoolProperty::New(metadata->GetDeviceIsVideoOnly()));
  this->SetProperty(mitk::USImageMetadata::PROP_PROBE_NAME, mitk::StringProperty::New(metadata->GetProbeName())); 
  this->SetProperty(mitk::USImageMetadata::PROP_PROBE_FREQUENCY, mitk::StringProperty::New(metadata->GetProbeFrequency())); 
  this->SetProperty(mitk::USImageMetadata::PROP_ZOOM, mitk::StringProperty::New(metadata->GetZoom())); 
}

