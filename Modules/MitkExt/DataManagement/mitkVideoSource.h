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

#ifndef _mitk_Video_Source_h_
#define _mitk_Video_Source_h_

#include "mitkCommon.h"
#include "MitkExtExports.h"
#include <itkObject.h>
#include <itkObjectFactory.h>
#include <mitkMessage.h>

namespace mitk
{
  ///
  /// Simple base class for acquiring video data. 
  ///
  class MitkExt_EXPORT VideoSource : public itk::Object
  {
    public:
      ///
      /// Smart pointer defs
      ///
      mitkClassMacro( VideoSource, itk::Object );
      ///
      /// assigns the grabbing devices for acquiring the next frame. 
      /// in this base implementation it does nothing except incrementing
      /// m_FrameCount
      ///
      virtual void FetchFrame();
      ///
      /// \return a pointer to the image data array for opengl rendering. 
      ///
      virtual unsigned char * GetVideoTexture() = 0;
      ///
      /// advices this class to start the video capturing.
      /// in this base implementation: toggles m_CapturingInProcess, resets m_FrameCount
      /// *ATTENTION*: this should be also done in subclasses overwriting this method
      ///
      virtual void StartCapturing();
      ///
      /// advices this class to stop the video capturing.
      /// in this base implementation: toggles m_CapturingInProcess, resets m_FrameCount
      /// *ATTENTION*: this should be also done in subclasses overwriting this method
      ///
      virtual void StopCapturing();
      ///
      /// \return true if video capturing is active.
      /// \see m_CapturingInProcess
      ///
      virtual bool IsCapturingEnabled() const;
      ///
      /// \return the current frame width (might be 0 if unknown)
      ///
      virtual int GetImageWidth();
      ///
      /// \return the current frame height (might be 0 if unknown)
      ///
      virtual int GetImageHeight();
      ///
      /// \return the current frame count
      ///
      virtual unsigned long GetFrameCount() const;

  protected:
      ///
      /// init member
      ///
      VideoSource();
      ///
      /// deletes m_CurrentVideoTexture (if not 0)
      ///
      virtual ~VideoSource();  

      ///
      /// finally this is what the video source must create: a video texture pointer
      ///
      unsigned char * m_CurrentVideoTexture;

      ///
      /// should be filled when the first frame is available
      ///
      int m_CaptureWidth;

      ///
      /// should be filled when the first frame is available
      ///
      int m_CaptureHeight;

      ///
      /// saves if capturing is in procress
      ///
      bool m_CapturingInProcess;

      ///
      /// Saves the current frame count. Incremented in FetchFrame(). 
      /// Resetted to 0 when StartCapturing() or StopCapturing() is called.
      ///
      unsigned long m_FrameCount;
 
  };
}
#endif // Header


