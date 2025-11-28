#pragma once

#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Particles/ParticleProperty.hpp"

namespace axdl::primedep::MetroidPrime {
class ParticleElectric final
: public TypedResource('ELSC', ".elsm", ".elsm.elsc", DESCRIPTION("Particle Electric Data")),
  particles::IPropertyContainer {
public:
  ParticleElectric(const char* ptr, std::size_t size);
  explicit ParticleElectric(const nlohmann::ordered_json& in);
  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  uint32_t immediateChildCount() const override;
  std::optional<std::vector<std::shared_ptr<IResource>>> immediateChildren() const override;
  std::optional<std::vector<ObjectTag32Big>> allChildTags() const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }

  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  ParticleElectric();
  particles::IntElementProperty m_particleSystemLifetime;
  particles::IntElementProperty m_segmentLifetime;
  particles::RealElementProperty m_generationRate;
  particles::IntElementProperty m_instanceCount;
  particles::IntElementProperty m_segmentCount;
  particles::ColorElementProperty m_color;
  particles::EmitterElementProperty m_initialEmitter;
  particles::EmitterElementProperty m_finalEmitter;
  particles::RealElementProperty m_amplitudeLength;
  particles::RealElementProperty m_amplitudeDirect;
  particles::RealElementProperty m_lineWidth1;
  particles::RealElementProperty m_lineWidth2;
  particles::RealElementProperty m_lineWidth3;
  particles::ColorElementProperty m_lineColor1;
  particles::ColorElementProperty m_lineColor2;
  particles::ColorElementProperty m_lineColor3;
  particles::AssetID32BigElementProperty m_childSwoosh;
  particles::AssetID32BigElementProperty m_startParticle;
  particles::AssetID32BigElementProperty m_endParticle;
  particles::BoolElementProperty m_zeroY;
};
} // namespace axdl::primedep::MetroidPrime