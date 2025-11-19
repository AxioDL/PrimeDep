#pragma once

#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Particles/ParticleProperty.hpp"
namespace axdl::primedep ::MetroidPrime {
class ParticleSwoosh final : public TypedResource('SWHC', ".swsh", ".swsh.swhc", DESCRIPTION("Particle Swoosh Data")),
                             particles::IPropertyContainer {
public:
  ParticleSwoosh(const char* ptr, std::size_t size);
  ParticleSwoosh(const nlohmann::ordered_json& in);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  ParticleSwoosh();
  particles::IntElementProperty m_particleSystemLifetime;
  particles::RealElementProperty m_timeMultiplier;
  particles::RealElementProperty m_leftRadius;
  particles::RealElementProperty m_rightRadius;
  particles::IntElementProperty m_length;
  particles::ColorElementProperty m_color;
  particles::IntElementProperty m_side;
  particles::RealElementProperty m_initialRotation;
  particles::RealElementProperty m_masterRotation;
  particles::VectorElementProperty m_particleOffset;
  particles::VectorElementProperty m_initialVelocity;
  particles::VectorElementProperty m_nextPosition;
  particles::ModVectorElementProperty m_velocity1;
  particles::ModVectorElementProperty m_velocity2;
  particles::IntElementProperty m_splineSegments;
  particles::UVElementProperty m_texture;
  particles::IntElementProperty m_textureSpan;
  particles::BoolElementProperty m_leftRadiusForRight;
  particles::BoolElementProperty m_crossSection;
  particles::BoolElementProperty m_localVelocity1;
  particles::BoolElementProperty m_localVelocity2;
  particles::BoolElementProperty m_singleRotation;
  particles::BoolElementProperty m_wireframe;
  particles::UVElementProperty m_wireframeTexture;
  particles::BoolElementProperty m_additiveAlpha;
  particles::BoolElementProperty m_enableZBuffer;
  particles::BoolElementProperty m_viewOrient;
  particles::BoolElementProperty m_useClockRandom;
};
} // namespace axdl::primedep::MetroidPrime