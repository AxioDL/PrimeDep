#include "PrimeDep/Resources/MetroidPrime/Particle.hpp"

#include "PrimeDep/Particles/ColorElement.hpp"
#include "PrimeDep/Particles/EmitterElement.hpp"
#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ModVectorElement.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include "PrimeDep/Particles/UVElement.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

#include "PrimeDep/Resources/MetroidPrime/Model.hpp"
#include "PrimeDep/Resources/MetroidPrime/ParticleElectric.hpp"
#include "PrimeDep/Resources/MetroidPrime/ParticleSwoosh.hpp"
#include "athena/FileReader.hpp"
#include "athena/MemoryReader.hpp"

#include <iostream>

namespace axdl::primedep::MetroidPrime {

Particle::Particle()
: m_particleSystemLifetime{FOURCC('PSLT'), "ParticleSystemLifetime", this}
, m_particleSystemWarmupTime{FOURCC('PSWT'), "ParticleSystemWarmupTime", this}
, m_particleSystemTimeScale{FOURCC('PSTS'), "ParticleSystemTimeScale", this}
, m_particleSystemOffset{FOURCC('POFS'), "ParticleSystemOffset", this}
, m_particleSystemSeed{FOURCC('SEED'), "ParticleSystemSeed", this}
, m_lineLength{FOURCC('LENG'), "LineLength", this}
, m_lineWidth{FOURCC('WIDT'), "LineWidth", this}
, m_maxParticles{FOURCC('MAXP'), "MaxParticles", this}
, m_particleColor{FOURCC('COLR'), "ParticleColor", this}
, m_generationRate{FOURCC('GRTE'), "GenerationRate", this}
, m_particleLifetime{FOURCC('LTME'), "ParticleLifetime", this}
, m_emitter{FOURCC('EMTR'), "Emitter", this}
, m_lines{kDefaultLines, FOURCC('LINE'), "Lines", this}
, m_scaleLeadingLinePoint{kDefaultScaleLeadingLinePoint, FOURCC('FXLL'), "ScaleLeadingLinePoint", this}
, m_additiveAlpha{kDefaultAdditiveAlpha, FOURCC('AAPH'), "AdditiveAlpha", this}
, m_enableZBuffer{kDefaultEnableZBuffer, FOURCC('ZBUF'), "EnableZBuffer", this}
, m_sort{kDefaultSort, FOURCC('SORT'), "EnableSort", this}
, m_enableLighting{kDefaultEnableLighting, FOURCC('LIT_'), "EnableLighting", this}

, m_orientToOrigin{kDefaultOrientToOrigin, FOURCC('ORNT'), "OrientToOrigin", this}

, m_rightVectorScaledOnParticle{kDefaultRightVectorScaledOnParticle, FOURCC('RSOP'), "RightVectorScaledOnParticle",
                                this}
, m_motionBlur{kDefaultMotionBlur, FOURCC('MBLR'), "MotionBlur", this}
, m_particleModelAdditiveAlpha{kDefaultParticleModelAdditiveAlpha, FOURCC('PMAB'), "ParticleModelAdditiveAlpha", this}
, m_particleModelUnorientedSquare{kDefaultParticleModelUnorientedSquare, FOURCC('PMUS'),
                                  "ParticleModelUnorientedSquare", this}
, m_particleModelOrientation{kDefaultParticleModelOrientation, FOURCC('PMOO'), "ParticleModelOrientation", this}
, m_vectorMod1Local{kDefaultVectorMod1Local, FOURCC('VMD1'), "VectorMod1Local", this}
, m_vectorMod2Local{kDefaultVectorMod2Local, FOURCC('VMD2'), "VectorMod2Local", this}
, m_vectorMod3Local{kDefaultVectorMod3Local, FOURCC('VMD3'), "VectorMod3Local", this}
, m_vectorMod4Local{kDefaultVectorMod4Local, FOURCC('VMD4'), "VectorMod4Local", this}
, m_colorIndirect{kDefaultColorIndirect, FOURCC('CIND'), "ColorIndirect", this}
, m_optionalSystem{kDefaultOptionalSystem, FOURCC('OPTS'), "OptionalSystem", this}
, m_motionBlurSamples{FOURCC('MBSP'), "MotionBlurSamples", this}
, m_particleSize{FOURCC('SIZE'), "ParticleSize", this}
, m_particleAngle{FOURCC('ROTA'), "ParticleAngle", this}
, m_texture{FOURCC('TEXR'), "Texture", this}
, m_indirectTexture{FOURCC('TIND'), "IndirectTexture", this}
, m_particleModel{Model::ResourceType(), FOURCC('PMDL'), "ParticleModel", this}
, m_particleModelOffset{FOURCC('PMOP'), "ParticleModelOffset", this}
, m_particleModelRotation{FOURCC('PMRT'), "ParticleModelRotation", this}
, m_particleModelScale{FOURCC('PMSC'), "ParticleModelScale", this}
, m_particleModelColor{FOURCC('PMCL'), "ParticleModelColor", this}
, m_particleVelocity1{FOURCC('VEL1'), "ParticleVelocity1", this}
, m_particleVelocity2{FOURCC('VEL2'), "ParticleVelocity2", this}
, m_particleVelocity3{FOURCC('VEL3'), "ParticleVelocity3", this}
, m_particleVelocity4{FOURCC('VEL4'), "ParticleVelocity4", this}
, m_countedChildSystem{ResourceType(), FOURCC('ICTS'), "CountedChildSystem", this}
, m_childSystemSpawnCount{FOURCC('NCSY'), "ChildSystemSpawnCount", this}
, m_childSystemSpawnFrame{FOURCC('CSSD'), "ChildSystemSpawnFrame", this}
, m_doneChildSystem{ResourceType(), FOURCC('IDTS'), "DoneChildSystem", this}
, m_doneChildSystemSpawnCount{FOURCC('NDSY'), "DoneSystemSpawnCount", this}
, m_intervalChildSystem{ResourceType(), FOURCC('IITS'), "IntervalChildSystem", this}
, m_intervalChildSystemSpawnInterval{FOURCC('PISY'), "IntervalChildSystemSpawnInterval", this}
, m_intervalChildSystemSpawnFrame{FOURCC('SISY'), "IntervalChildSystemSpawnFrame", this}
, m_spawnSystems{FOURCC('KSSM'), "SpawnSystems", this}
, m_childSwooshSystem{ParticleSwoosh::ResourceType(), FOURCC('SSWH'), "ChildSwooshSystem", this}
, m_childSwooshSystemSpawnFrame{FOURCC('SSSD'), "ChildSwooshSpawnFrame", this}
, m_childSwooshSystemOffset{FOURCC('SSPO'), "ChildSwooshSystemOffset", this}
, m_childElectricSystem{ParticleElectric::ResourceType(), FOURCC('SELC'), "ChildElectricSystem", this}
, m_childElectricSystemSpawnFrame{FOURCC('SESD'), "ChildElectricSystemSpawnFrame", this}
, m_childElectricSystemOffset{FOURCC('SEPO'), "ChildElectricSystemOffset", this}
, m_lightType{FOURCC('LTYP'), "LightType", this}
, m_lightColor{FOURCC('LCLR'), "LightColor", this}
, m_lightIntensity{FOURCC('LINT'), "LightIntensity", this}
, m_lightOffset{FOURCC('LOFF'), "LightOffset", this}
, m_lightDirection{FOURCC('LDIR'), "LightDirection", this}
, m_lightFallOffType{FOURCC('LFOT'), "LightFallOffType", this}
, m_lightFallOffRadius{FOURCC('LFOR'), "LightFallOffRadius", this}
, m_lightCutOffAngle{FOURCC('LSLA'), "LightCutOffAngle", this}
, m_accessParameter1{FOURCC('ADV1'), "AccessParameter1", this}
, m_accessParameter2{FOURCC('ADV2'), "AccessParameter2", this}
, m_accessParameter3{FOURCC('ADV3'), "AccessParameter3", this}
, m_accessParameter4{FOURCC('ADV4'), "AccessParameter4", this}
, m_accessParameter5{FOURCC('ADV5'), "AccessParameter5", this}
, m_accessParameter6{FOURCC('ADV6'), "AccessParameter6", this}
, m_accessParameter7{FOURCC('ADV7'), "AccessParameter7", this}
, m_accessParameter8{FOURCC('ADV8'), "AccessParameter8", this}
, m_initialLocation{FOURCC('ILOC'), "OldInitialLocation", this}
, m_initialVector{FOURCC('IVEC'), "OldInitialVector", this}
, m_particleSystemInitialVelocity{FOURCC('PSIV'), "OldParticleSystemInitialVelocity", this}
, m_particleSystemVelocityMod{FOURCC('PSVM'), "OldParticleSystemVelocityMod", this}
, m_particleSystemOrientationVelocity{FOURCC('PSOV'), "OldParticleSystemOrientationVelocity", this} {}

Particle::Particle(const char* ptr, const std::size_t size) : Particle() {
  athena::io::MemoryReader in(ptr, size, true);

  if (particles::ParticleDataFactory::GetClassID(in) != FOURCC('GPSM')) {
    return;
  }
  loadParticleProperties(in);
}

Particle::Particle(const nlohmann::ordered_json& in) : Particle() {
  for (int loadOrder = 0; const auto& [key, _] : in.items()) {
    const auto property = propertyForName(key);
    if (!property) {
      continue;
    }

    property->loadValue(in);
    property->setLoadOrder(loadOrder);
    ++loadOrder;
  }

  sortProperties();
}

Particle::~Particle() {}

std::shared_ptr<IResource> Particle::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<Particle>(ptr, size);
}

std::shared_ptr<IResource> Particle::ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
  const auto p = GetRawPath(path);
  athena::io::FileReader reader(p.generic_string());
  nlohmann::ordered_json in = nlohmann::ordered_json::parse(reader.readString());
  return std::make_shared<Particle>(in);
}

[[nodiscard]] bool Particle::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);

  nlohmann::ordered_json data = nlohmann::ordered_json::object();
  for (const auto& property : m_properties) {
    if (property->loadOrder() == -1) {
      continue;
    }

    property->PutTo(data);
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = data.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

[[nodiscard]] bool Particle::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);

  athena::io::FileWriter writer(p.generic_string());
  particles::ParticleDataFactory::SetClassID(writer, FOURCC('GPSM'));
  for (const auto& property : m_properties) {
    if (property->loadOrder() == -1) {
      continue;
    }
    property->PutTo(writer);
  }
  particles::ParticleDataFactory::SetClassID(writer, FOURCC('_END'));

  return !writer.hasError();
}

void Particle::loadParticleProperties(athena::io::IStreamReader& reader) {
  auto classId = particles::ParticleDataFactory::GetClassID(reader);

  int loadOrder = 0;
  while (classId != FOURCC('_END') && !reader.hasError()) {
    if (auto* cls = propertyForClass(classId)) {
      cls->loadValue(reader);
      cls->setLoadOrder(loadOrder);
      ++loadOrder;
    } else {
      std::cout << "Unhandled class " << classId.toString() << std::endl;
    }
    classId = particles::ParticleDataFactory::GetClassID(reader);
  }

  sortProperties();
}

} // namespace axdl::primedep::MetroidPrime