#pragma once

#include "PASDatabase.hpp"
#include "PrimeDep/Math/AABox.hpp"
#include "PrimeDep/Resources/MetroidPrime/SkinnedModel.hpp"

#include <nlohmann/json_fwd.hpp>

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io
namespace axdl::primedep::animations {
class CharacterInfo {
public:
  class AnimInfo {
  public:
    explicit AnimInfo(athena::io::IStreamReader& in);
    explicit AnimInfo(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;

  private:
    int32_t m_index;
    std::string m_a;
    std::string m_b;
  };

  class ParticleResData {
  public:
    explicit ParticleResData(athena::io::IStreamReader& in, int tableCount);
    explicit ParticleResData(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;

  private:
    std::vector<AssetId32Big> m_particles;
    std::vector<AssetId32Big> m_swooshes;
    std::vector<AssetId32Big> m_electrics1;
    std::optional<std::vector<AssetId32Big>> m_electrics2;
  };

  explicit CharacterInfo(athena::io::IStreamReader& in);
  explicit CharacterInfo(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const;
  void PutTo(nlohmann::ordered_json& out) const;

private:
  uint16_t m_tableCount;
  std::string m_name;
  AssetId32Big m_modelId;
  AssetId32Big m_skinId;

  std::vector<AnimInfo> m_animInfos;
  PASDatabase m_pasDatabase;
  ParticleResData m_particleResData;
  uint32_t m_unk;
  std::vector<std::pair<std::string, AABox>> m_boundingBoxes;
  std::vector<int32_t> m_animationIndices;
};
} // namespace axdl::primedep::animations