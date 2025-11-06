#pragma once

#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Particles/SpawnSystemKeyframeData.hpp"

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

  [[nodiscard]] bool writeUncooked(std::string_view path) const override;
  [[nodiscard]] bool writeCooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }

private:
  void loadParticleProperties(athena::io::IStreamReader & reader);

  std::unique_ptr<particles::IntElement> m_particleSystemLifetime;
  std::unique_ptr<particles::IntElement> m_particleSystemWarmupTime;
  std::unique_ptr<particles::RealElement> m_particleSystemTimeScale;
  std::unique_ptr<particles::VectorElement> m_particleSystemOffset;
  std::unique_ptr<particles::IntElement> m_particleSystemSeed;
  std::unique_ptr<particles::RealElement> m_lineLength;
  std::unique_ptr<particles::RealElement> m_lineWidth;
  std::unique_ptr<particles::IntElement> m_maxParticles;
  std::unique_ptr<particles::ColorElement> m_particleColor;
  std::unique_ptr<particles::RealElement> m_generationRate;
  std::unique_ptr<particles::IntElement> m_particleLifetime;
  std::unique_ptr<particles::EmitterElement> m_emitter;
  static constexpr bool kDefaultLines = false;
  bool m_lines : 1 = kDefaultLines;
  static constexpr bool kDefaultScaleLeadingLinePoint = false;
  bool m_scaleLeadingLinePoint : 1 = kDefaultScaleLeadingLinePoint;
  static constexpr bool kDefaultAdditiveAlpha = false;
  bool m_additiveAlpha : 1 = kDefaultAdditiveAlpha;
  static constexpr bool kDefaultEnableZBuffer = false;
  bool m_enableZBuffer : 1 = false;
  static constexpr bool kDefaultSort = false;
  bool m_sort : 1 = kDefaultSort;
  static constexpr bool kDefaultEnableLighting = false;
  bool m_enableLighting : 1 = kDefaultEnableLighting;
  static constexpr bool kDefaultOrientToOrigin = false;
  bool m_orientToOrigin : 1 = kDefaultOrientToOrigin;
  static constexpr bool kDefaultRightVectorScaledOnParticle = false;
  bool m_rightVectorScaledOnParticle : 1 = kDefaultRightVectorScaledOnParticle;
  static constexpr bool kDefaultMotionBlur = false;
  bool m_motionBlur : 1 = kDefaultMotionBlur;
  static constexpr bool kDefaultParticleModelAdditiveAlpha = false;
  bool m_particleModelAdditiveAlpha : 1 = kDefaultParticleModelAdditiveAlpha;
  static constexpr bool kDefaultParticleModelUnorientedSquare = false;
  bool m_particleModelUnorientedSquare : 1 = kDefaultParticleModelUnorientedSquare;
  static constexpr bool kDefaultParticleModelOrientation = true;
  bool m_particleModelOrientation : 1 = kDefaultParticleModelOrientation;
  static constexpr bool kDefaultVectorMod1Local = false;
  bool m_vectorMod1Local : 1 = kDefaultVectorMod1Local;
  static constexpr bool kDefaultVectorMod2Local = false;
  bool m_vectorMod2Local : 1 = kDefaultVectorMod2Local;
  static constexpr bool kDefaultVectorMod3Local = false;
  bool m_vectorMod3Local : 1 = kDefaultVectorMod3Local;
  static constexpr bool kDefaultVectorMod4Local = false;
  bool m_vectorMod4Local : 1 = kDefaultVectorMod4Local;
  static constexpr bool kDefaultColorIndirect = false;
  bool m_colorIndirect : 1 = kDefaultColorIndirect;
  static constexpr bool kDefaultOptionalSystem = false;
  bool m_optionalSystem : 1 = kDefaultOptionalSystem;
  std::unique_ptr<particles::IntElement> m_motionBlurSamples;
  std::unique_ptr<particles::RealElement> m_particleSize;
  std::unique_ptr<particles::RealElement> m_particleAngle;
  std::unique_ptr<particles::UVElement> m_texture;
  std::unique_ptr<particles::UVElement> m_indirectTexture;
  std::optional<AssetId32Big> m_particleModel;
  std::unique_ptr<particles::VectorElement> m_particleModelOffset;
  std::unique_ptr<particles::VectorElement> m_particleModelRotation;
  std::unique_ptr<particles::VectorElement> m_particleModelScale;
  std::unique_ptr<particles::ColorElement> m_particleModelColor;
  std::unique_ptr<particles::ModVectorElement> m_particleVelocity1;
  std::unique_ptr<particles::ModVectorElement> m_particleVelocity2;
  std::unique_ptr<particles::ModVectorElement> m_particleVelocity3;
  std::unique_ptr<particles::ModVectorElement> m_particleVelocity4;
  std::optional<AssetId32Big> m_countedChildSystem;
  std::unique_ptr<particles::IntElement> m_childSystemSpawnCount;
  std::unique_ptr<particles::IntElement> m_childSystemSpawnFrame;
  std::optional<AssetId32Big> m_doneChildSystem;
  std::unique_ptr<particles::IntElement> m_doneChildSystemSpawnCount;
  std::unique_ptr<particles::IntElement> m_doneChildSystemSpawnFrame;
  std::optional<AssetId32Big> m_intervalChildSystem;
  std::unique_ptr<particles::IntElement> m_intervalChildSystemSpawnInterval;
  std::unique_ptr<particles::IntElement> m_intervalChildSystemSpawnFrame;
  std::optional<particles::SpawnSystemKeyframeData> m_spawnSystems;
  std::optional<AssetId32Big> m_childSwooshSystem;
  std::unique_ptr<particles::IntElement> m_childSwooshSystemSpawnFrame;
  std::unique_ptr<particles::VectorElement> m_childSwooshSystemOffset;
  std::optional<AssetId32Big> m_childElectricSystem;
  std::unique_ptr<particles::IntElement> m_childElectricSystemSpawnFrame;
  std::unique_ptr<particles::VectorElement> m_childElectricSystemOffset;
  std::unique_ptr<particles::IntElement> m_lightType;
  std::unique_ptr<particles::ColorElement> m_lightColor;
  std::unique_ptr<particles::RealElement> m_lightIntensity;
  std::unique_ptr<particles::VectorElement> m_lightOffset;
  std::unique_ptr<particles::VectorElement> m_lightDirection;
  std::unique_ptr<particles::IntElement> m_lightFallOffType;
  std::unique_ptr<particles::RealElement> m_lightFallOffRadius;
  std::unique_ptr<particles::RealElement> m_lightCutOffAngle;
  std::unique_ptr<particles::RealElement> m_accessParameter1;
  std::unique_ptr<particles::RealElement> m_accessParameter2;
  std::unique_ptr<particles::RealElement> m_accessParameter3;
  std::unique_ptr<particles::RealElement> m_accessParameter4;
  std::unique_ptr<particles::RealElement> m_accessParameter5;
  std::unique_ptr<particles::RealElement> m_accessParameter6;
  std::unique_ptr<particles::RealElement> m_accessParameter7;
  std::unique_ptr<particles::RealElement> m_accessParameter8;
};
} // namespace axdl::primedep::MetroidPrime