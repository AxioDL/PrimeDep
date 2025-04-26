#pragma once
#include "nlohmann/json_fwd.hpp"
#include "PrimeDep/ObjectTag.hpp"

namespace axdl::primedep {
class ParticleData {
public:
  enum class EParentedMode {
    Initial,
    ContinuousEmitter,
    ContinuousSystem,
  };

  ParticleData() = default;
  ParticleData(const ObjectTag32Big& tag, const std::string_view boneName, const float scale, const EParentedMode mode)
  : x4_particle(tag), xc_boneName(boneName), x1c_scale(scale), x20_parentMode(mode) {}
  explicit ParticleData(athena::io::IStreamReader& in);
  uint32_t GetDuration() const { return x0_duration; }
  const ObjectTag32Big& GetTag() const { return x4_particle; }
  std::string_view GetSegmentName() const { return xc_boneName; }
  float GetScale() const { return x1c_scale; }
  EParentedMode GetParentedMode() const { return x20_parentMode; }

  void PutTo(nlohmann::ordered_json& j) const;

private:
  uint32_t x0_duration = 0;
  ObjectTag32Big x4_particle;
  std::string xc_boneName = "root";
  float x1c_scale = 1.f;
  EParentedMode x20_parentMode = EParentedMode::Initial;
};
} // namespace axdl::primedep