#pragma once
#include "IResourceSource.hpp"

namespace axdl::primedep {
class AssetId32Big;
template <class AssetIdType>
class ModelSkinPool {
public:
  void addModel(AssetIdType assetId) {
    if (std::ranges::find_if(m_models, [&assetId](const Status& model) { return model.assetId == assetId; }) !=
        m_models.end()) {
      return;
    }
    m_models.emplace_back(assetId);
  }

  void addSkin(AssetIdType assetId) {
    if (std::ranges::find_if(m_skins, [&assetId](const Status& skin) { return skin.assetId == assetId; }) !=
        m_skins.end()) {
      return;
    }
    m_skins.emplace_back(assetId);
  }

  static ModelSkinPool& instance() {
    static ModelSkinPool instance;
    return instance;
  }

private:
  struct Status {
    AssetIdType assetId;
    bool processed{false};
  };
  std::vector<Status> m_models;
  std::vector<Status> m_skins;
};

using ModelSkinPool32Big = ModelSkinPool<AssetId32Big>;
} // namespace axdl::primedep