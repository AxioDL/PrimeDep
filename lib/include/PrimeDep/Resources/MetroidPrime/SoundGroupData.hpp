#pragma once
#include "PrimeDep/AssetId.hpp"

namespace axdl::primedep::MetroidPrime {
class SoundGroupData {
public:
  explicit SoundGroupData(athena::io::IStreamReader& in) : m_groupId(in.readUint32Big()), m_assetId(in) {}

private:
  uint32_t m_groupId;
  AssetId32Big m_assetId;
};
} // namespace axdl::primedep::MetroidPrime