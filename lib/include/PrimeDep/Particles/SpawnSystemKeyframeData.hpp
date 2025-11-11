#pragma once

#include "PrimeDep/AssetId.hpp"

#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>
#include <nlohmann/json_fwd.hpp>

namespace axdl::primedep::particles {
class SpawnSystemKeyframeData {
public:
  class SpawnSystemKeyframeInfo {
  public:
    explicit SpawnSystemKeyframeInfo(athena::io::IStreamReader& in);
    explicit SpawnSystemKeyframeInfo(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;

  private:
    AssetId32Big m_particleSystemId;
    uint32_t m_unknown1;
    uint32_t m_unknown2;
    uint32_t m_unknown3;
  };

  SpawnSystemKeyframeData() = default;
  explicit SpawnSystemKeyframeData(athena::io::IStreamReader& in);
  explicit SpawnSystemKeyframeData(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const;
  void PutTo(nlohmann::ordered_json& out) const;

private:
  bool m_valid{};
  uint32_t m_unknown1{};
  uint32_t m_unknown2{};
  uint32_t m_endFrame{};
  uint32_t m_unknown3{};
  std::vector<std::pair<uint32_t, std::vector<SpawnSystemKeyframeInfo>>> m_frames;
};
}; // namespace axdl::primedep::particles