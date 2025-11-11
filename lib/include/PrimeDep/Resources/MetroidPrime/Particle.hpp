#pragma once

#include "Model.hpp"
#include "ParticleElectric.hpp"
#include "ParticleSwoosh.hpp"
#include "PrimeDep/IResource.hpp"

#include "PrimeDep/Particles/ParticleProperty.hpp"
#include "PrimeDep/Particles/SpawnSystemKeyframeData.hpp"

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
  ~Particle() override;

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
  particles::IntElementProperty m_particleSystemLifetime{FOURCC('PSLT'), "ParticleSystemLifetime", this};
  particles::IntElementProperty m_particleSystemWarmupTime{FOURCC('PSWT'), "ParticleSystemWarmupTime", this};
  particles::RealElementProperty m_particleSystemTimeScale{FOURCC('PSTS'), "ParticleSystemTimeScale", this};
  particles::VectorElementProperty m_particleSystemOffset{FOURCC('POFS'), "ParticleSystemOffset", this};
  particles::IntElementProperty m_particleSystemSeed{FOURCC('SEED'), "ParticleSystemSeed", this};
  particles::RealElementProperty m_lineLength{FOURCC('LENG'), "LineLength", this};
  particles::RealElementProperty m_lineWidth{FOURCC('WIDT'), "LineWidth", this};
  particles::IntElementProperty m_maxParticles{FOURCC('MAXP'), "MaxParticles", this};
  particles::ColorElementProperty m_particleColor{FOURCC('PCOL'), "ParticleColor", this};
  particles::RealElementProperty m_generationRate{FOURCC('GRTE'), "GenerationRate", this};
  particles::IntElementProperty m_particleLifetime{FOURCC('LTME'), "ParticleLifetime", this};
  particles::EmitterElementProperty m_emitter{FOURCC('EMTR'), "Emitter", this};
  static constexpr bool kDefaultLines = false;
  particles::BoolElementProperty m_lines{kDefaultLines, FOURCC('LINE'), "Lines", this};
  static constexpr bool kDefaultScaleLeadingLinePoint = false;
  particles::BoolElementProperty m_scaleLeadingLinePoint{kDefaultScaleLeadingLinePoint, FOURCC('FXLL'),
                                                         "ScaleLeadingLinePoint", this};
  static constexpr bool kDefaultAdditiveAlpha = false;
  particles::BoolElementProperty m_additiveAlpha{kDefaultAdditiveAlpha, FOURCC('AAPH'), "AdditiveAlpha", this};
  static constexpr bool kDefaultEnableZBuffer = false;
  particles::BoolElementProperty m_enableZBuffer{kDefaultAdditiveAlpha, FOURCC('ZBUF'), "EnableZBuffer", this};
  static constexpr bool kDefaultSort = false;
  particles::BoolElementProperty m_sort{kDefaultAdditiveAlpha, FOURCC('SORT'), "EnableSort", this};
  static constexpr bool kDefaultEnableLighting = false;
  particles::BoolElementProperty m_enableLighting{kDefaultAdditiveAlpha, FOURCC('LIT_'), "EnableLighting", this};
  static constexpr bool kDefaultOrientToOrigin = false;
  particles::BoolElementProperty m_orientToOrigin{kDefaultAdditiveAlpha, FOURCC('ORNT'), "OrientToOrigin", this};
  static constexpr bool kDefaultRightVectorScaledOnParticle = false;
  particles::BoolElementProperty m_rightVectorScaledOnParticle{kDefaultAdditiveAlpha, FOURCC('RSOP'),
                                                               "RightVectorScaledOnParticle", this};
  static constexpr bool kDefaultMotionBlur = false;
  particles::BoolElementProperty m_motionBlur{kDefaultAdditiveAlpha, FOURCC('MBLR'), "MotionBlur", this};
  static constexpr bool kDefaultParticleModelAdditiveAlpha = false;
  particles::BoolElementProperty m_particleModelAdditiveAlpha{kDefaultAdditiveAlpha, FOURCC('PMAB'),
                                                              "ParticleModelAdditiveAlpha", this};
  static constexpr bool kDefaultParticleModelUnorientedSquare = false;
  particles::BoolElementProperty m_particleModelUnorientedSquare{kDefaultAdditiveAlpha, FOURCC('PMUS'),
                                                                 "ParticleModelUnorientedSquare", this};
  static constexpr bool kDefaultParticleModelOrientation = true;
  particles::BoolElementProperty m_particleModelOrientation{kDefaultAdditiveAlpha, FOURCC('PMOO'),
                                                            "ParticleModelOrientation", this};
  static constexpr bool kDefaultVectorMod1Local = false;
  particles::BoolElementProperty m_vectorMod1Local{kDefaultVectorMod1Local, FOURCC('VMD1'), "VectorMod1Local", this};
  static constexpr bool kDefaultVectorMod2Local = false;
  particles::BoolElementProperty m_vectorMod2Local{kDefaultVectorMod2Local, FOURCC('VMD2'), "VectorMod2Local", this};
  static constexpr bool kDefaultVectorMod3Local = false;
  particles::BoolElementProperty m_vectorMod3Local{kDefaultVectorMod3Local, FOURCC('VMD3'), "VectorMod3Local", this};
  static constexpr bool kDefaultVectorMod4Local = false;
  particles::BoolElementProperty m_vectorMod4Local{kDefaultVectorMod4Local, FOURCC('VMD4'), "VectorMod4Local", this};
  static constexpr bool kDefaultColorIndirect = false;
  particles::BoolElementProperty m_colorIndirect{kDefaultColorIndirect, FOURCC('CIND'), "ColorIndirect", this};
  static constexpr bool kDefaultOptionalSystem = false;
  particles::BoolElementProperty m_optionalSystem{kDefaultOptionalSystem, FOURCC('OPTS'), "OptionalSystem", this};
  particles::IntElementProperty m_motionBlurSamples{FOURCC('MBSP'), "MotionBlurSamples", this};
  particles::RealElementProperty m_particleSize{FOURCC('SIZE'), "ParticleSize", this};
  particles::RealElementProperty m_particleAngle{FOURCC('ROTA'), "ParticleAngle", this};
  particles::UVElementProperty m_texture{FOURCC('TEXR'), "Texture", this};
  particles::UVElementProperty m_indirectTexture{FOURCC('TIND'), "IndirectTexture", this};
  particles::AssetID32BigElementProperty m_particleModel{Model::ResourceType(), FOURCC('PMDL'), "ParticleModel", this};
  particles::VectorElementProperty m_particleModelOffset{FOURCC('PMOP'), "ParticleModelOffset", this};
  particles::VectorElementProperty m_particleModelRotation{FOURCC('PMRT'), "ParticleModelOrientation", this};
  particles::VectorElementProperty m_particleModelScale{FOURCC('PMSC'), "ParticleModelScale", this};
  particles::ColorElementProperty m_particleModelColor{FOURCC('PMCL'), "ParticleModelColor", this};
  particles::ModVectorElementProperty m_particleVelocity1{FOURCC('VEL1'), "ParticleVelocity1", this};
  particles::ModVectorElementProperty m_particleVelocity2{FOURCC('VEL2'), "ParticleVelocity2", this};
  particles::ModVectorElementProperty m_particleVelocity3{FOURCC('VEL3'), "ParticleVelocity3", this};
  particles::ModVectorElementProperty m_particleVelocity4{FOURCC('VEL4'), "ParticleVelocity4", this};
  particles::AssetID32BigElementProperty m_countedChildSystem{ResourceType(), FOURCC('ICTS'), "CountedChildSystem",
                                                              this};
  particles::IntElementProperty m_childSystemSpawnCount{FOURCC('NCSY'), "ChildSystemSpawnCount", this};
  particles::IntElementProperty m_childSystemSpawnFrame{FOURCC('CSSD'), "ChildSystemSpawnFrame", this};
  particles::AssetID32BigElementProperty m_doneChildSystem{ResourceType(), FOURCC('IDTS'), "DoneChildSystem", this};
  particles::IntElementProperty m_doneChildSystemSpawnCount{FOURCC('NDSY'), "DoneSystemSpawnCount", this};
  particles::AssetID32BigElementProperty m_intervalChildSystem{ResourceType(), FOURCC('IITS'), "IntervalChildSystem",
                                                               this};
  particles::IntElementProperty m_intervalChildSystemSpawnInterval{FOURCC('PISY'), "IntervalChildSystemSpawnInterval",
                                                                   this};
  particles::IntElementProperty m_intervalChildSystemSpawnFrame{FOURCC('SISY'), "IntervalChildSystemSpawnFrame", this};
  particles::SpawnSystemKeyframeDataElementProperty m_spawnSystems{FOURCC('KSSM'), "SpawnSystems", this};
  particles::AssetID32BigElementProperty m_childSwooshSystem{ParticleSwoosh::ResourceType(), FOURCC('SSWH'),
                                                             "ChildSwooshSystem", this};
  particles::IntElementProperty m_childSwooshSystemSpawnFrame{FOURCC('SSSD'), "ChildSwooshSpawnFrame", this};
  particles::VectorElementProperty m_childSwooshSystemOffset{FOURCC('SSPO'), "ChildSwooshSystemOffset", this};
  particles::AssetID32BigElementProperty m_childElectricSystem{ParticleElectric::ResourceType(), FOURCC('SELC'),
                                                               "ChildElectricSystem", this};
  particles::IntElementProperty m_childElectricSystemSpawnFrame{FOURCC('SESD'), "ChildElectricSystemSpawnFrame", this};
  particles::VectorElementProperty m_childElectricSystemOffset{FOURCC('SEPO'), "ChildElectricSystemOffset", this};
  particles::IntElementProperty m_lightType{FOURCC('LTYP'), "LightType", this};
  particles::ColorElementProperty m_lightColor{FOURCC('LCLR'), "LightColor", this};
  particles::RealElementProperty m_lightIntensity{FOURCC('LINT'), "LightIntensity", this};
  particles::VectorElementProperty m_lightOffset{FOURCC('LOFF'), "LightOffset", this};
  particles::VectorElementProperty m_lightDirection{FOURCC('LDIR'), "LightDirection", this};
  particles::IntElementProperty m_lightFallOffType{FOURCC('LFOT'), "LightFallOffType", this};
  particles::RealElementProperty m_lightFallOffRadius{FOURCC('LFOR'), "LightFallOffRadius", this};
  particles::RealElementProperty m_lightCutOffAngle{FOURCC('LSLA'), "LightCutOffAngle", this};
  particles::RealElementProperty m_accessParameter1{FOURCC('ADV1'), "AccessParameter1", this};
  particles::RealElementProperty m_accessParameter2{FOURCC('ADV2'), "AccessParameter2", this};
  particles::RealElementProperty m_accessParameter3{FOURCC('ADV3'), "AccessParameter3", this};
  particles::RealElementProperty m_accessParameter4{FOURCC('ADV4'), "AccessParameter4", this};
  particles::RealElementProperty m_accessParameter5{FOURCC('ADV5'), "AccessParameter5", this};
  particles::RealElementProperty m_accessParameter6{FOURCC('ADV6'), "AccessParameter6", this};
  particles::RealElementProperty m_accessParameter7{FOURCC('ADV7'), "AccessParameter7", this};
  particles::RealElementProperty m_accessParameter8{FOURCC('ADV8'), "AccessParameter8", this};
  std::vector<particles::IParticleProperty*> m_loadOrder;
};
} // namespace axdl::primedep::MetroidPrime