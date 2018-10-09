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

#ifndef MITKCONTROLPOINTMANAGER_H
#define MITKCONTROLPOINTMANAGER_H

// semantic relations module
#include "mitkSemanticTypes.h"
#include <MitkSemanticRelationsExports.h>

// mitk core
#include <mitkDataNode.h>

namespace mitk
{
  /**
  * @brief Provides helper functions that are needed to work with control points.
  *
  *   These functions help to generate new control points, check for overlapping / containing control points or provide functionality
  *   to find a fitting control point or even extend an already existing control point.
  */
  /**
  * @brief Generates a control point from a given data node.
  *        The date is extracted from the data node by using the 'DICOMHelper::GetDICOMDateFromDataNode'-function.
  *
  * @par datanode   A data node pointer, whose date should be included in the newly generated control point.
  */
  MITKSEMANTICRELATIONS_EXPORT SemanticTypes::ControlPoint GenerateControlPoint(const mitk::DataNode* datanode);
  /**
  * @brief Find and return a whole control point including its date given a specific control point UID.
  *
  * @param  controlPointUID     The control point UID as string.
  * @param  allControlPoints    All currently known control points of a specific case.
  *
  * @return   The control point with its UID and the date.
  */
  MITKSEMANTICRELATIONS_EXPORT SemanticTypes::ControlPoint GetControlPointByUID(const SemanticTypes::ID& controlPointUID, const std::vector<SemanticTypes::ControlPoint>& allControlPoints);
  /**
  * @brief Returns a string that displays the given control point in the format "YYYY-MM-DD".
  *        This function is used in the GUI to display the control point as header in the "information-type - control-point"-matrix.
  *
  * @par controlPoint   The control point to convert into a string.
  */
  MITKSEMANTICRELATIONS_EXPORT std::string GetControlPointAsString(const SemanticTypes::ControlPoint& controlPoint);
  /**
  * @brief Returns an already existing control point from the given vector of control points. This existing control point has the
  *        the same date (year, month, day) as the given single control point.
  *        If no existing control point can be found an empty control point is returned.
  *
  * @par controlPoint       The control point to check for existence.
  * @par allControlPoints   The vector of already existing control points.
  */
  MITKSEMANTICRELATIONS_EXPORT SemanticTypes::ControlPoint FindExistingControlPoint(const SemanticTypes::ControlPoint& controlPoint, const std::vector<SemanticTypes::ControlPoint>& allControlPoints);
  /**
  * @brief Returns an already existing close control point from the given vector of control points. This closest control point has a date
  *        date that is within a certain distance-in-days to the given control point.
  *        If no closest control point can be found within the distance threshold an empty control point is returned.
  *
  * @par controlPoint       The control point to check for distance.
  * @par allControlPoints   The vector of already existing control points.
  */
  MITKSEMANTICRELATIONS_EXPORT SemanticTypes::ControlPoint FindClosestControlPoint(const SemanticTypes::ControlPoint& controlPoint, std::vector<SemanticTypes::ControlPoint>& allControlPoints);
  /**
  * @brief Returns the examination period to which the given control point belongs.
  *        Each examination point holds a vector of control point UIDs so that the UID of the given control point can be compared against the UIDs of the vector.
  *        An empty examination period is returned if,
  *        - the given vector of examination periods is empty
  *        - the examination periods do not contain any control point UIDs
  *        - the UID of the given control point is not contained in any examination period
  */
  MITKSEMANTICRELATIONS_EXPORT SemanticTypes::ExaminationPeriod FindExaminationPeriod(const SemanticTypes::ControlPoint& controlPoint, const std::vector<SemanticTypes::ExaminationPeriod>& allExaminationPeriods);
} // namespace mitk

#endif // MITKCONTROLPOINTMANAGER_H
