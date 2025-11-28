#include "PrimeDep/Resources/MetroidPrime/SoundGroupData.hpp"

#include "PrimeDep/Resources/MetroidPrime/AudioGroup.hpp"

namespace axdl::primedep::MetroidPrime {
SoundGroupData::SoundGroupData(athena::io::IStreamReader& in)
: m_groupId(in.readUint32Big()), m_assetId(in, AudioGroup::ResourceType()) {}

SoundGroupData::SoundGroupData(const nlohmann::ordered_json& in)
: m_groupId(in.value("GroupID", m_groupId))
, m_assetId(in.value("AssetID", nlohmann::ordered_json::object()), AudioGroup::ResourceType()) {}

void SoundGroupData::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32(m_groupId);
  m_assetId.PutTo(out);
}

void SoundGroupData::PutTo(nlohmann::ordered_json& out) const {
  out["GroupID"] = m_groupId;
  m_assetId.PutTo(out["Ref"], AudioGroup::ResourceType());
}

} // namespace axdl::primedep::MetroidPrime