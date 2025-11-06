#pragma once

#include "PrimeDep/Animation/POINode.hpp"
#include "PrimeDep/Particles/ParticleData.hpp"

namespace axdl::primedep {
class ParticlePOINode final : public POINode {
public:
  ParticlePOINode();
  explicit ParticlePOINode(athena::io::IStreamReader& in);
  explicit ParticlePOINode(const nlohmann::ordered_json& in);
  [[nodiscard]] const ParticleData& GetParticleData() const { return x38_data; }

  static ParticlePOINode CopyNodeMinusStartTime(const ParticlePOINode& node, const CharAnimTime& startTime);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& j) const override;

private:
  ParticleData x38_data;
};
} // namespace axdl::primedep