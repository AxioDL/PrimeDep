#pragma once

#include "PrimeDep/Resources/MetroidPrime/Model.hpp"
#include "PrimeDep/Resources/MetroidPrime/SkinRules.hpp"

namespace axdl::primedep::animations {
template <class AssetIdType>
class SkinnedModel {
public:
  struct ModelPair {
    AssetIdType modelId;
    AssetIdType skinId;
  };

  const AssetIdType& modelId() const { return m_baseModel.modelId; }
  void setModelId(const AssetIdType& modelId) { m_baseModel.modelId = modelId; }

  const AssetIdType& skinId() const { return m_baseModel.skinId; }
  void setSkinId(const AssetIdType& skinId) { m_baseModel.skinId = skinId; }

  const AssetIdType& rigId() const { return m_rigId; }
  void setRigId(const AssetIdType& rigId) { m_rigId = rigId; }

  void addModel(const AssetIdType& modelId, const AssetIdType& skinId) { m_models.emplace_back(modelId, skinId); }

  void addOverlay(const FourCC type, const AssetIdType& modelId, const AssetIdType& skinId) {
    m_overlays[type] = {modelId, skinId};
  }

  bool cookModels(std::string_view path) { return false; }

  bool uncookModel(std::string_view path) { return false; }

private:
  ModelPair m_baseModel;
  AssetIdType m_rigId;
  std::vector<ModelPair> m_models;
  std::map<FourCC, ModelPair> m_overlays;
};

using SkinnedModel32Big = SkinnedModel<AssetId32Big>;
} // namespace axdl::primedep::animations