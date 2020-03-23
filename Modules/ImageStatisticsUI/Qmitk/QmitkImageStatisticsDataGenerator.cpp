/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "QmitkImageStatisticsDataGenerator.h"

#include "mitkImageStatisticsContainer.h"
#include "mitkStatisticsToImageRelationRule.h"
#include "mitkStatisticsToMaskRelationRule.h"
#include "mitkNodePredicateFunction.h"
#include "mitkNodePredicateAnd.h"
#include "mitkNodePredicateDataProperty.h"
#include "mitkProperties.h"
#include "mitkImageStatisticsContainerManager.h"

#include "QmitkImageStatisticsCalculationRunnable.h"

void QmitkImageStatisticsDataGenerator::SetIgnoreZeroValueVoxel(bool _arg)
{
  if (m_IgnoreZeroValueVoxel != _arg)
  {
    m_IgnoreZeroValueVoxel = _arg;
    this->EnsureRecheckingAndGeneration();
  }
}

bool QmitkImageStatisticsDataGenerator::GetIgnoreZeroValueVoxel() const
{
  return this->m_IgnoreZeroValueVoxel;
}

void QmitkImageStatisticsDataGenerator::SetHistogramNBins(unsigned int nbins)
{
  if (m_HistogramNBins != nbins)
  {
    m_HistogramNBins = nbins;
    this->EnsureRecheckingAndGeneration();
  }
}

unsigned int QmitkImageStatisticsDataGenerator::GetHistogramNBins() const
{
  return this->m_HistogramNBins;
}

bool QmitkImageStatisticsDataGenerator::NodeChangeIsRelevant(const mitk::DataNode* changedNode) const
{
  auto result = QmitkImageAndRoiDataGeneratorBase::NodeChangeIsRelevant(changedNode);

  if (!result)
  {
    std::string status;
    if (!changedNode->GetStringProperty(mitk::STATS_GENERATION_STATUS_PROPERTY_NAME.c_str(), status) || status != mitk::STATS_GENERATION_STATUS_VALUE_WORK_IN_PROGRESS)
    {
      auto stats = dynamic_cast<const mitk::ImageStatisticsContainer*>(changedNode->GetData());
      return stats != nullptr;
    }
  }
  return false;
}

bool QmitkImageStatisticsDataGenerator::IsValidResultAvailable(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode) const
{
  auto resultNode = this->GetLatestResult(imageNode, roiNode, true, true);
  return resultNode.IsNotNull();
}

mitk::DataNode::Pointer QmitkImageStatisticsDataGenerator::GetLatestResult(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode, bool onlyIfUpToDate, bool noWIP) const
{
  mitk::DataNode::Pointer result;

  auto storage = m_Storage.Lock();
  if (storage)
  {
    if (!imageNode || !imageNode->GetData())
    {
      mitkThrow() << "Image is nullptr";
    }

    const auto image = imageNode->GetData();
    const mitk::BaseData* mask = nullptr;
    if (roiNode)
    {
      mask = roiNode->GetData();
    }

    mitk::NodePredicateBase::ConstPointer predicate = mitk::ImageStatisticsContainerManager::GetStatisticsPredicateForSources(image, mask);

    if (predicate)
    {
      auto binPredicate = mitk::NodePredicateDataProperty::New(mitk::STATS_HISTOGRAM_BIN_PROPERTY_NAME.c_str(), mitk::UIntProperty::New(m_HistogramNBins));
      auto zeroPredicate = mitk::NodePredicateDataProperty::New(mitk::STATS_IGNORE_ZERO_VOXEL_PROPERTY_NAME.c_str(), mitk::BoolProperty::New(m_IgnoreZeroValueVoxel));

      predicate = mitk::NodePredicateAnd::New(predicate, binPredicate, zeroPredicate);

      if (noWIP)
      {
        predicate = mitk::NodePredicateAnd::New(predicate, mitk::NodePredicateDataProperty::New(mitk::STATS_GENERATION_STATUS_PROPERTY_NAME.c_str()));
      }

      std::shared_lock<std::shared_mutex> mutexguard(m_DataMutex);
      auto statisticContainerCandidateNodes = storage->GetSubset(predicate);

      auto statisticContainerCandidateNodesFiltered = mitk::DataStorage::SetOfObjects::New();

      for (const auto& node : *statisticContainerCandidateNodes)
      {
        auto isUpToDate = image->GetMTime() > node->GetData()->GetMTime()
                          && (mask == nullptr || mask->GetMTime() > node->GetData()->GetMTime());

        if (!onlyIfUpToDate || isUpToDate)
        {
          statisticContainerCandidateNodesFiltered->push_back(node);
        }
      }

      if (!statisticContainerCandidateNodesFiltered->empty())
      {
        auto newestElement = statisticContainerCandidateNodesFiltered->front();
        if (statisticContainerCandidateNodesFiltered->size() > 1)
        {
          //in case of multiple found statistics, return only newest one
          auto newestIter = std::max_element(std::begin(*statisticContainerCandidateNodesFiltered), std::end(*statisticContainerCandidateNodesFiltered), [](mitk::DataNode::Pointer a, mitk::DataNode::Pointer b) {
            return a->GetData()->GetMTime() < b->GetData()->GetMTime();
            });
          newestElement = *newestIter;
          MITK_WARN << "multiple statistics (" << statisticContainerCandidateNodesFiltered->size() << ") for image/mask found. Returning only newest one.";
          for (const auto& node : *statisticContainerCandidateNodesFiltered)
          {
            MITK_DEBUG << node->GetName() << ", timestamp: " << node->GetData()->GetMTime();
          }
        }
        result = newestElement;
      }
    }
  }

  return result;
}

void QmitkImageStatisticsDataGenerator::IndicateFutureResults(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode) const
{
  if (!imageNode || !imageNode->GetData())
  {
    mitkThrow() << "Image node is nullptr";
  }

  auto image = dynamic_cast<const mitk::Image*>(imageNode->GetData());
  if (!image)
  {
    mitkThrow() << "Image node date is nullptr or no image.";
  }

  const mitk::BaseData* mask = nullptr;
  if (roiNode)
  {
    mask = roiNode->GetData();
  }

  if (!this->IsValidResultAvailable(imageNode, roiNode))
  {
    auto dummyStats = mitk::ImageStatisticsContainer::New();

    auto imageRule = mitk::StatisticsToImageRelationRule::New();
    imageRule->Connect(dummyStats, image);

    if (nullptr != mask)
    {
      auto maskRule = mitk::StatisticsToMaskRelationRule::New();
      maskRule->Connect(dummyStats, mask);
    }

    dummyStats->SetProperty(mitk::STATS_HISTOGRAM_BIN_PROPERTY_NAME.c_str(), mitk::UIntProperty::New(m_HistogramNBins));
    dummyStats->SetProperty(mitk::STATS_IGNORE_ZERO_VOXEL_PROPERTY_NAME.c_str(), mitk::BoolProperty::New(m_IgnoreZeroValueVoxel));

    auto dummyNode = CreateWIPDataNode(dummyStats, "WIP_"+GenerateStatisticsNodeName(image, mask));

    auto storage = m_Storage.Lock();
    if (storage)
    {
      std::shared_lock<std::shared_mutex> mutexguard(m_DataMutex);
      storage->Add(dummyNode);
    }
  }
}

QmitkDataGenerationJobBase* QmitkImageStatisticsDataGenerator::GetNextMissingGenerationJob(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode) const
{
  auto resultDataNode = this->GetLatestResult(imageNode, roiNode, true, false);

  if (resultDataNode.IsNull())
  {
    if (!imageNode || !imageNode->GetData())
    {
      mitkThrow() << "Image node is nullptr";
    }

    auto image = dynamic_cast<const mitk::Image*>(imageNode->GetData());
    if (!image)
    {
      mitkThrow() << "Image node date is nullptr or no image.";
    }

    const mitk::Image* mask = nullptr;
    const mitk::PlanarFigure* planar = nullptr;
    if (roiNode)
    {
      mask = dynamic_cast<const mitk::Image*>(roiNode->GetData());
      planar = dynamic_cast<const mitk::PlanarFigure*>(roiNode->GetData());
    }

    auto newJob = new QmitkImageStatisticsCalculationRunnable;

    newJob->Initialize(image, mask, planar);
    newJob->SetIgnoreZeroValueVoxel(m_IgnoreZeroValueVoxel);

    return newJob;
  }

  return nullptr;
}

void QmitkImageStatisticsDataGenerator::RemoveObsoleteDataNodes(const mitk::DataNode* imageNode, const mitk::DataNode* roiNode) const
{
  if (!imageNode || !imageNode->GetData())
  {
    mitkThrow() << "Image is nullptr";
  }

  const auto image = imageNode->GetData();
  const mitk::BaseData* mask = nullptr;
  if (roiNode)
  {
    mask = roiNode->GetData();
  }

  auto lastResult = this->GetLatestResult(imageNode, roiNode, false, false);

  auto rulePredicate = mitk::ImageStatisticsContainerManager::GetStatisticsPredicateForSources(image, mask);
  auto notLatestPredicate = mitk::NodePredicateFunction::New([lastResult](const mitk::DataNode* node) { return node != lastResult; });
  auto binPredicate = mitk::NodePredicateDataProperty::New(mitk::STATS_HISTOGRAM_BIN_PROPERTY_NAME.c_str(), mitk::UIntProperty::New(m_HistogramNBins));
  auto zeroPredicate = mitk::NodePredicateDataProperty::New(mitk::STATS_IGNORE_ZERO_VOXEL_PROPERTY_NAME.c_str(), mitk::BoolProperty::New(m_IgnoreZeroValueVoxel));

  mitk::NodePredicateBase::ConstPointer predicate = mitk::NodePredicateAnd::New(predicate, notLatestPredicate);
  predicate = mitk::NodePredicateAnd::New(predicate, binPredicate, zeroPredicate);

  auto storage = m_Storage.Lock();
  if (storage)
  {
    std::shared_lock<std::shared_mutex> mutexguard(m_DataMutex);

    auto oldStatisticContainerNodes = storage->GetSubset(predicate);
    storage->Remove(oldStatisticContainerNodes);
  }
}

mitk::DataNode::Pointer QmitkImageStatisticsDataGenerator::PrepareResultForStorage(const std::string& label, mitk::BaseData* result, const QmitkDataGenerationJobBase* job) const
{
  auto statsJob = dynamic_cast<const QmitkImageStatisticsCalculationRunnable*>(job);

  if (statsJob)
  {
    auto resultNode = mitk::DataNode::New();
    resultNode->SetProperty("helper object", mitk::BoolProperty::New(true));
    resultNode->SetVisibility(false);
    resultNode->SetData(result);
    
    const mitk::BaseData* roi = statsJob->GetMaskImage();
    if (!roi)
    {
      roi = statsJob->GetPlanarFigure();
    }
    resultNode->SetName(this->GenerateStatisticsNodeName(statsJob->GetStatisticsImage(), roi));

    return resultNode;
  }

  return nullptr;
}

std::string QmitkImageStatisticsDataGenerator::GenerateStatisticsNodeName(const mitk::Image* image, const mitk::BaseData* roi) const
{
  std::stringstream statisticsNodeName;
  statisticsNodeName << "statistics_bins-" << m_HistogramNBins <<"_";

  if (m_IgnoreZeroValueVoxel)
  {
    statisticsNodeName << "noZeros_";
  }

  if (!image)
  {
    mitkThrow() << "Image is nullptr";
  }

  statisticsNodeName << image->GetUID();

  if (roi)
  {
    statisticsNodeName << "_" + roi->GetUID();
  }

  return statisticsNodeName.str();
}
