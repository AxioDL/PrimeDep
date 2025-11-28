#pragma once
#include "PrimeDep/AssetId.hpp"

namespace axdl::primedep::MetroidPrime {
class SoundGroupData {
public:
  explicit SoundGroupData(athena::io::IStreamReader& in);
  explicit SoundGroupData(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const;
  void PutTo(nlohmann::ordered_json& out) const;

private:
  uint32_t m_groupId;
  AssetId32Big m_assetId;
};
} // namespace axdl::primedep::MetroidPrime