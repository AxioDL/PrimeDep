#pragma once

#include "PrimeDep/Resources/MetroidPrime/Model.hpp"
#include "PrimeDep/Resources/MetroidPrime/SkinRules.hpp"

namespace axdl::primedep::MetroidPrime {
class SkinnedModel {
public:
  struct ModelPair {
    AssetId32Big modelId;
    AssetId32Big skinId;
  };
  explicit SkinnedModel(athena::io::IStreamReader& in);
  explicit SkinnedModel(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const;
  void PutTo(nlohmann::ordered_json& out) const;

  const AssetId32Big& modelId() const { return m_baseModel.modelId; }
  void setModelId(const AssetId32Big& modelId) { m_baseModel.modelId = modelId; }

  const AssetId32Big& skinId() const { return m_baseModel.skinId; }
  void setSkinId(const AssetId32Big& skinId) { m_baseModel.skinId = skinId; }

  const AssetId32Big& rigId() const { return m_rigId; }
  void setRigId(const AssetId32Big& rigId) { m_rigId = rigId; }

  void addModel(const AssetId32Big& modelId, const AssetId32Big& skinId) { m_modelIds.emplace_back(modelId, skinId); }

  bool cookModels(std::string_view path) { return false; }

  bool uncookModel(std::string_view path) { return false; }

private:
  ModelPair m_baseModel;
  AssetId32Big m_rigId;
  std::vector<ModelPair> m_modelIds;
};
} // namespace axdl::primedep::MetroidPrime