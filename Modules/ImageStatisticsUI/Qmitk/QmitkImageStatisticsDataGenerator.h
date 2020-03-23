/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef __QMITK_IMAGE_STATISTICS_DATA_GENERATOR_H
#define __QMITK_IMAGE_STATISTICS_DATA_GENERATOR_H

#include "QmitkImageAndRoiDataGeneratorBase.h"

#include <MitkImageStatisticsUIExports.h>

/*!
\brief QmitkImageStatisticsDataGenerator
*/
class MITKIMAGESTATISTICSUI_EXPORT QmitkImageStatisticsDataGenerator : public QmitkImageAndRoiDataGeneratorBase
{
public:
  QmitkImageStatisticsDataGenerator(mitk::DataStorage::Pointer storage, QObject* parent = nullptr) : QmitkImageAndRoiDataGeneratorBase(storage, parent) {};
  QmitkImageStatisticsDataGenerator(QObject* parent = nullptr) : QmitkImageAndRoiDataGeneratorBase(parent) {};

  bool IsValidResultAvailable(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode) const final;

  /** Returns the latest result for a given image and ROI and the current settings of the generator.
   @param onlyIfUpToDate Indicates if results should only be returned if the are up to date, thus not older then image and ROI.
   @param noWIP If noWIP is true, the function only returns valid final result and not just its placeholder (WIP).
   If noWIP equals false it might also return a WIP, thus the valid result is currently processed/ordered but might not be ready yet.*/
  mitk::DataNode::Pointer GetLatestResult(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode, bool onlyIfUpToDate = false, bool noWIP = true) const;

  std::string GenerateStatisticsNodeName(const mitk::Image* image, const mitk::BaseData* roi) const;

  /*! /brief Set flag to ignore zero valued voxels */
  void SetIgnoreZeroValueVoxel(bool _arg);
  /*! /brief Get status of zero value voxel ignoring. */
  bool GetIgnoreZeroValueVoxel() const;

  /*! /brief Set bin size for histogram resolution.*/
  void SetHistogramNBins(unsigned int nbins);
  /*! /brief Get bin size for histogram resolution.*/
  unsigned int GetHistogramNBins() const;

protected:
  bool NodeChangeIsRelevant(const mitk::DataNode* changedNode) const final;
  void IndicateFutureResults(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode) const final;
  QmitkDataGenerationJobBase* GetNextMissingGenerationJob(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode) const final;
  void RemoveObsoleteDataNodes(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode) const final;
  mitk::DataNode::Pointer PrepareResultForStorage(const std::string& label, mitk::BaseData* result, const QmitkDataGenerationJobBase* job) const final;

  QmitkImageStatisticsDataGenerator(const QmitkImageStatisticsDataGenerator&) = delete;
  QmitkImageStatisticsDataGenerator& operator = (const QmitkImageStatisticsDataGenerator&) = delete;

  bool m_IgnoreZeroValueVoxel = false;
  unsigned int m_HistogramNBins = 100;
};

#endif
