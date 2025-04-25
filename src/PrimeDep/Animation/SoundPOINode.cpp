#include "PrimeDep/Animation/SoundPOINode.hpp"

namespace axdl::primedep {

SoundPOINode::SoundPOINode()
: POINode("root", EPOIType::Sound, CharAnimTime(), -1, false, 1.f, -1, 0)
, x38_sfxId(0)
, x3c_falloff(0.f)
, x40_maxDist(0.f) {}

SoundPOINode::SoundPOINode(athena::io::IStreamReader& in)
: POINode(in), x38_sfxId(in.readUint32Big()), x3c_falloff(in.readFloatBig()), x40_maxDist(in.readFloatBig()) {}

SoundPOINode::SoundPOINode(const std::string_view name, const EPOIType type, const CharAnimTime& time,
                           const uint32_t index, const bool unique, const float weight, const int32_t charIdx,
                           const int32_t flags, const uint32_t sfxId, const float falloff, const float maxDist)
: POINode(name, type, time, index, unique, weight, charIdx, flags)
, x38_sfxId(sfxId)
, x3c_falloff(falloff)
, x40_maxDist(maxDist) {}

SoundPOINode SoundPOINode::CopyNodeMinusStartTime(const SoundPOINode& node, const CharAnimTime& startTime) {
  SoundPOINode ret = node;
  ret.x1c_time -= startTime;
  return ret;
}

void SoundPOINode::PutTo(nlohmann::ordered_json& j) const {
  POINode::PutTo(j);
  j["SoundID"] = x38_sfxId;
  j["Falloff"] = x3c_falloff;
  j["MaxDist"] = x40_maxDist;
}
} // namespace axdl::primedep
