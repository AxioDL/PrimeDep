#pragma once

#include "PrimeDep/IResource.hpp"

#include "PrimeDep/Particles/ParticleProperty.hpp"

namespace axdl::primedep::particles {
class IElement;
class BoolElement;
class IntElement;
class RealElement;
class VectorElement;
class EmitterElement;
class ColorElement;
class UVElement;
class ModVectorElement;
} // namespace axdl::primedep::particles
namespace axdl::primedep::MetroidPrime {
class Particle final : public TypedResource('PART', ".gpsm", ".gpsm.part", DESCRIPTION("Particle Data")),
                       particles::IPropertyContainer {
public:
  Particle(const char* ptr, std::size_t size);
  explicit Particle(const nlohmann::ordered_json& in);
  ~Particle() override;

  uint32_t immediateChildCount() const override;

  std::optional<std::vector<std::shared_ptr<IResource>>> immediateChildren() const override;
  std::optional<std::vector<ObjectTag32Big>> allChildTags() const override;

  [[nodiscard]] bool writeUncooked(std::string_view path) const override;
  [[nodiscard]] bool writeCooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  static void addChildRes(std::vector<std::shared_ptr<IResource>> children,
                          const particles::AssetID32BigElementProperty& resId);
  Particle();
  void loadParticleProperties(athena::io::IStreamReader & reader);
  particles::IntElementProperty m_particleSystemLifetime;
  particles::IntElementProperty m_particleSystemWarmupTime;
  particles::RealElementProperty m_particleSystemTimeScale;
  particles::VectorElementProperty m_particleSystemOffset;
  particles::IntElementProperty m_particleSystemSeed;
  particles::RealElementProperty m_lineLength;
  particles::RealElementProperty m_lineWidth;
  particles::IntElementProperty m_maxParticles;
  particles::ColorElementProperty m_particleColor;
  particles::RealElementProperty m_generationRate;
  particles::IntElementProperty m_particleLifetime;
  particles::EmitterElementProperty m_emitter;
  static constexpr bool kDefaultLines = false;
  particles::BoolElementProperty m_lines;
  static constexpr bool kDefaultScaleLeadingLinePoint = false;
  particles::BoolElementProperty m_scaleLeadingLinePoint;
  static constexpr bool kDefaultAdditiveAlpha = false;
  particles::BoolElementProperty m_additiveAlpha;
  static constexpr bool kDefaultEnableZBuffer = false;
  particles::BoolElementProperty m_enableZBuffer;
  static constexpr bool kDefaultSort = false;
  particles::BoolElementProperty m_sort;
  static constexpr bool kDefaultEnableLighting = false;
  particles::BoolElementProperty m_enableLighting;
  static constexpr bool kDefaultOrientToOrigin = false;
  particles::BoolElementProperty m_orientToOrigin;
  static constexpr bool kDefaultRightVectorScaledOnParticle = false;
  particles::BoolElementProperty m_rightVectorScaledOnParticle;
  static constexpr bool kDefaultMotionBlur = false;
  particles::BoolElementProperty m_motionBlur;
  static constexpr bool kDefaultParticleModelAdditiveAlpha = false;
  particles::BoolElementProperty m_particleModelAdditiveAlpha;
  static constexpr bool kDefaultParticleModelUnorientedSquare = false;
  particles::BoolElementProperty m_particleModelUnorientedSquare;
  static constexpr bool kDefaultParticleModelOrientation = true;
  particles::BoolElementProperty m_particleModelOrientation;
  static constexpr bool kDefaultVectorMod1Local = false;
  particles::BoolElementProperty m_vectorMod1Local;
  static constexpr bool kDefaultVectorMod2Local = false;
  particles::BoolElementProperty m_vectorMod2Local;
  static constexpr bool kDefaultVectorMod3Local = false;
  particles::BoolElementProperty m_vectorMod3Local;
  static constexpr bool kDefaultVectorMod4Local = false;
  particles::BoolElementProperty m_vectorMod4Local;
  static constexpr bool kDefaultColorIndirect = false;
  particles::BoolElementProperty m_colorIndirect;
  static constexpr bool kDefaultOptionalSystem = false;
  particles::BoolElementProperty m_optionalSystem;
  particles::IntElementProperty m_motionBlurSamples;
  particles::RealElementProperty m_particleSize;
  particles::RealElementProperty m_particleAngle;
  particles::UVElementProperty m_texture;
  particles::UVElementProperty m_indirectTexture;
  particles::AssetID32BigElementProperty m_particleModel;
  particles::VectorElementProperty m_particleModelOffset;
  particles::VectorElementProperty m_particleModelRotation;
  particles::VectorElementProperty m_particleModelScale;
  particles::ColorElementProperty m_particleModelColor;
  particles::ModVectorElementProperty m_particleVelocity1;
  particles::ModVectorElementProperty m_particleVelocity2;
  particles::ModVectorElementProperty m_particleVelocity3;
  particles::ModVectorElementProperty m_particleVelocity4;
  particles::AssetID32BigElementProperty m_countedChildSystem;
  particles::IntElementProperty m_childSystemSpawnCount;
  particles::IntElementProperty m_childSystemSpawnFrame;
  particles::AssetID32BigElementProperty m_doneChildSystem;
  particles::IntElementProperty m_doneChildSystemSpawnCount;
  particles::AssetID32BigElementProperty m_intervalChildSystem;
  particles::IntElementProperty m_intervalChildSystemSpawnInterval;
  particles::IntElementProperty m_intervalChildSystemSpawnFrame;
  particles::SpawnSystemKeyframeDataElementProperty m_spawnSystems;
  particles::AssetID32BigElementProperty m_childSwooshSystem;
  particles::IntElementProperty m_childSwooshSystemSpawnFrame;
  particles::VectorElementProperty m_childSwooshSystemOffset;
  particles::AssetID32BigElementProperty m_childElectricSystem;
  particles::IntElementProperty m_childElectricSystemSpawnFrame;
  particles::VectorElementProperty m_childElectricSystemOffset;
  particles::IntElementProperty m_lightType;
  particles::ColorElementProperty m_lightColor;
  particles::RealElementProperty m_lightIntensity;
  particles::VectorElementProperty m_lightOffset;
  particles::VectorElementProperty m_lightDirection;
  particles::IntElementProperty m_lightFallOffType;
  particles::RealElementProperty m_lightFallOffRadius;
  particles::RealElementProperty m_lightCutOffAngle;
  particles::RealElementProperty m_accessParameter1;
  particles::RealElementProperty m_accessParameter2;
  particles::RealElementProperty m_accessParameter3;
  particles::RealElementProperty m_accessParameter4;
  particles::RealElementProperty m_accessParameter5;
  particles::RealElementProperty m_accessParameter6;
  particles::RealElementProperty m_accessParameter7;
  particles::RealElementProperty m_accessParameter8;

  // Old unused properties, for compatibility’s sake
  particles::VectorElementProperty m_initialLocation;
  particles::VectorElementProperty m_initialVector;
  particles::VectorElementProperty m_particleSystemInitialVelocity;
  particles::ModVectorElementProperty m_particleSystemVelocityMod;
  particles::VectorElementProperty m_particleSystemOrientationVelocity;
};
} // namespace axdl::primedep::MetroidPrime