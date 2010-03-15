/*=========================================================================

 Program:   Medical Imaging & Interaction Toolkit
 Language:  C++
 Date:      $Date: 2009-11-02 20:38:30 +0100 (Mo, 02 Nov 2009) $
 Version:   $Revision: 19650 $

 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#include "mitkSegmentationSelectionProvider.h"
#include <mitkDataNodeSelection.h>
#include "berrySelectionChangedEvent.h"


namespace mitk
{


SegmentationSelectionProvider::SegmentationSelectionProvider()
: m_CurrentSelection(new DataNodeSelection)

{
}


void SegmentationSelectionProvider::AddSelectionChangedListener(berry::ISelectionChangedListener::Pointer listener)
{
  m_RegisteredListeners.AddListener(listener);
}

void SegmentationSelectionProvider::RemoveSelectionChangedListener(berry::ISelectionChangedListener::Pointer listener)
{
  m_RegisteredListeners.RemoveListener(listener);
}

berry::ISelection::ConstPointer SegmentationSelectionProvider::GetSelection() const
{
 return m_CurrentSelection;
}

void SegmentationSelectionProvider::SetSelection(berry::ISelection::Pointer selection)
{
}

void SegmentationSelectionProvider::FireSelectionChanged(DataNodeSelection::Pointer selection)
{
  if(selection.IsNotNull())
  {
  
    m_CurrentSelection = selection;
    berry::SelectionChangedEvent::Pointer event(new berry::SelectionChangedEvent(berry::ISelectionProvider::Pointer(this), m_CurrentSelection));
    m_RegisteredListeners.selectionChanged(event);
  }
}

void SegmentationSelectionProvider::FireSelectionChanged(DataNode::Pointer selectedNode)
{
  this->FireSelectionChanged(DataNodeSelection::Pointer(new DataNodeSelection(selectedNode)));
}


}