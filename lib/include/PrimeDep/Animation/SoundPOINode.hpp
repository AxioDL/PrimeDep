#pragma once

#include "PrimeDep/Animation/POINode.hpp"
#include "PrimeDep/Particles/ParticleData.hpp"

namespace axdl::primedep {
class SoundPOINode final : public POINode {
public:
  SoundPOINode();
  explicit SoundPOINode(athena::io::IStreamReader& in);
  SoundPOINode(std::string_view name, EPOIType type, const CharAnimTime& time, uint32_t index, bool unique,
               float weight, int32_t charIdx, int32_t flags, uint32_t sfxId, float falloff, float maxDist);
  [[nodiscard]] uint32_t GetSfxId() const { return x38_sfxId; }
  [[nodiscard]] float GetFalloff() const { return x3c_falloff; }
  [[nodiscard]] float GetMaxDist() const { return x40_maxDist; }

  static SoundPOINode CopyNodeMinusStartTime(const SoundPOINode& node, const CharAnimTime& startTime);
  void PutTo(nlohmann::ordered_json& j) const override;
private:
  uint32_t x38_sfxId;
  float x3c_falloff;
  float x40_maxDist;
};
} // namespace axdl::primedep