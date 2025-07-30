#pragma once

#include "PrimeDep/IResource.hpp"
namespace axdl::primedep::particles {
class IntElement;
class RealElement;
class VectorElement;
class EmitterElement;
class ColorElement;
class UVElement;
class ModVectorElement;
} // namespace axdl::primedep::particles

namespace axdl::primedep::MetroidPrime {
class Particle final : public TypedResource('PART', ".gpsm", ".gpsm.part", DESCRIPTION("Particle Data")) {
public:
  Particle(const char* ptr, std::size_t size);
  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }

private:
  // std::unique_ptr<particles::IntElement> m_particleSystemLifetime;
  // std::unique_ptr<particles::IntElement> m_particleSystemWarmupTime;
  // std::unique_ptr<particles::RealElement> m_particleSystemTimeScale;
  // std::unique_ptr<particles::VectorElement> m_particleSystemOffset;
  // std::unique_ptr<particles::IntElement> m_particleSystemSeed;
  // std::unique_ptr<particles::RealElement> m_lineLength;
  // std::unique_ptr<particles::RealElement> m_lineWidth;
  // std::unique_ptr<particles::IntElement> m_maxParticles;
  // std::unique_ptr<particles::ColorElement> m_particleColor;
  // std::unique_ptr<particles::RealElement> m_generationRate;
  // std::unique_ptr<particles::IntElement> m_particleLifetime;
  // std::unique_ptr<particles::EmitterElement> m_emitter;
  // bool m_lines : 1 = false;
  // bool m_scaleLeadingLinePoint : 1 = false;
  // bool m_additiveAlpha : 1 = false;
  // bool m_enableZBuffer : 1 = false;
  // bool m_sort : 1 = false;
  // bool m_orientToOrigin : 1 = false;
  // bool m_rightVectorScaledOnParticle : 1 = false;
  // bool m_motionBlur : 1 = false;
  // bool m_particleModelAdditiveAlpha : 1 = false;
  // bool m_particleUnorientedSquare : 1 = false;
  // bool m_particleModelOrientation : 1 = true;
  // bool m_vectorMod1Local : 1 = false;
  // bool m_vectorMod2Local : 1 = false;
  // bool m_vectorMod3Local : 1 = false;
  // bool m_vectorMod4Local : 1 = false;
  // bool m_optionalSystem : 1 = false;
  // bool m_colorIndirect : 1 = false;
  // std::unique_ptr<particles::IntElement> m_motionBlurSamples;
  // std::unique_ptr<particles::RealElement> m_particleSize;
  // std::unique_ptr<particles::RealElement> m_particleAngle;
  // std::unique_ptr<particles::UVElement> m_texture;
  // std::unique_ptr<particles::UVElement> m_indirectTexture;
  // std::optional<AssetId32Big> m_particleModel;
  // std::unique_ptr<particles::VectorElement> m_particleModelOffset;
  // std::unique_ptr<particles::VectorElement> m_particleModelRotation;
  // std::unique_ptr<particles::VectorElement> m_particleModelScale;
  // std::unique_ptr<particles::VectorElement> m_particleModelColor;
  // std::unique_ptr<particles::ModVectorElement> m_particleVelocity1;
  // std::unique_ptr<particles::ModVectorElement> m_particleVelocity2;
  // std::unique_ptr<particles::ModVectorElement> m_particleVelocity3;
  // std::unique_ptr<particles::ModVectorElement> m_particleVelocity4;
  // std::optional<AssetId32Big> m_countedChildSystem;
  // std::unique_ptr<particles::IntElement> m_childSystemSpawnCount;
  // std::unique_ptr<particles::IntElement> m_childSystemSpawnFrame;
  // std::optional<AssetId32Big> m_doneChildSystem;
  // std::unique_ptr<particles::IntElement> m_doneChildSystemSpawnCount;
  // std::unique_ptr<particles::IntElement> m_doneChildSystemSpawnFrame;
  // std::optional<AssetId32Big> m_intervalChildSystem;
  // std::unique_ptr<particles::IntElement> m_intervalChildSystemSpawnCount;
  // std::unique_ptr<particles::IntElement> m_intervalChildSystemSpawnFrame;
};
} // namespace axdl::primedep::MetroidPrime