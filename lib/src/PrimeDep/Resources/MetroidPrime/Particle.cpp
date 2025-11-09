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
#include "athena/MemoryReader.hpp"

#include <iostream>

namespace axdl::primedep::MetroidPrime {
Particle::Particle(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  if (particles::ParticleDataFactory::GetClassID(in) != FOURCC('GPSM')) {
    return;
  }
  loadParticleProperties(in);
}

std::shared_ptr<IResource> Particle::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<Particle>(ptr, size);
}

[[nodiscard]] bool Particle::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json data = nlohmann::ordered_json::object();

  if (m_particleSystemLifetime) {
    m_particleSystemLifetime->PutTo(data["ParticleSystemLifetime"]);
  }

  if (m_particleSystemWarmupTime) {
    m_particleSystemWarmupTime->PutTo(data["ParticleSystemWarmupTime"]);
  }

  if (m_particleSystemTimeScale) {
    m_particleSystemTimeScale->PutTo(data["ParticleSystemTimeScale"]);
  }

  if (m_particleSystemOffset) {
    m_particleSystemOffset->PutTo(data["ParticleSystemOffset"]);
  }

  if (m_particleSystemSeed) {
    m_particleSystemSeed->PutTo(data["ParticleSystemSeed"]);
  }

  if (m_lineLength) {
    m_lineLength->PutTo(data["LineLength"]);
  }

  if (m_lineWidth) {
    m_lineWidth->PutTo(data["LineWidth"]);
  }

  if (m_maxParticles) {
    m_maxParticles->PutTo(data["MaxParticles"]);
  }

  if (m_particleColor) {
    m_particleColor->PutTo(data["ParticleColor"]);
  }

  if (m_generationRate) {
    m_generationRate->PutTo(data["GenerationRate"]);
  }

  if (m_particleLifetime) {
    m_particleLifetime->PutTo(data["ParticleLifetime"]);
  }

  if (m_lines != kDefaultLines) {
    data["Lines"] = m_lines;
  }

  if (m_scaleLeadingLinePoint != kDefaultScaleLeadingLinePoint) {
    data["ScaleLeadingLinePoint"] = m_scaleLeadingLinePoint;
  }

  if (m_additiveAlpha != kDefaultAdditiveAlpha) {
    data["AdditiveAlpha"] = m_additiveAlpha;
  }

  if (m_enableZBuffer != kDefaultEnableZBuffer) {
    data["EnableZBuffer"] = m_enableZBuffer;
  }

  if (m_sort != kDefaultSort) {
    data["Sort"] = m_sort;
  }

  if (m_enableLighting != kDefaultEnableLighting) {
    data["EnableLighting"] = m_enableLighting;
  }

  if (m_orientToOrigin != kDefaultOrientToOrigin) {
    data["OrientToOrigin"] = m_orientToOrigin;
  }

  if (m_rightVectorScaledOnParticle != kDefaultRightVectorScaledOnParticle) {
    data["RightVectorScaledOnParticle"] = m_rightVectorScaledOnParticle;
  }

  if (m_motionBlur != kDefaultMotionBlur) {
    data["MotionBlur"] = m_motionBlur;
  }

  if (m_particleModelAdditiveAlpha != kDefaultParticleModelAdditiveAlpha) {
    data["ParticleModelAdditiveAlpha"] = m_particleModelAdditiveAlpha;
  }

  if (m_particleModelUnorientedSquare != kDefaultParticleModelUnorientedSquare) {
    data["ParticleModelUnorientedSquare"] = m_particleModelUnorientedSquare;
  }

  if (m_particleModelOrientation != kDefaultParticleModelOrientation) {
    data["ParticleModelOrientation"] = m_particleModelOrientation;
  }

  if (m_vectorMod1Local != kDefaultVectorMod1Local) {
    data["VectorMod1Local"] = m_vectorMod1Local;
  }

  if (m_vectorMod2Local != kDefaultVectorMod2Local) {
    data["VectorMod2Local"] = m_vectorMod2Local;
  }

  if (m_vectorMod3Local != kDefaultVectorMod3Local) {
    data["VectorMod3Local"] = m_vectorMod3Local;
  }

  if (m_vectorMod4Local != kDefaultVectorMod4Local) {
    data["VectorMod4Local"] = m_vectorMod4Local;
  }

  if (m_colorIndirect != kDefaultColorIndirect) {
    data["ColorIndirect"] = m_colorIndirect;
  }

  if (m_optionalSystem != kDefaultOptionalSystem) {
    data["OptionalSystem"] = m_optionalSystem;
  }

  if (m_motionBlurSamples) {
    m_motionBlurSamples->PutTo(data["MotionBlurSamples"]);
  }

  if (m_particleSize) {
    m_particleSize->PutTo(data["ParticleSize"]);
  }

  if (m_particleAngle) {
    m_particleAngle->PutTo(data["ParticleAngle"]);
  }

  if (m_texture) {
    m_texture->PutTo(data["Texture"]);
  }

  if (m_indirectTexture) {
    m_indirectTexture->PutTo(data["IndirectTexture"]);
  }

  if (m_particleModel) {
    m_particleModel->PutTo(data["ParticleModel"], Model::ResourceType());
  }

  if (m_particleModelOffset) {
    m_particleModelOffset->PutTo(data["ParticleModelOffset"]);
  }

  if (m_particleModelRotation) {
    m_particleModelRotation->PutTo(data["ParticleModelRotation"]);
  }

  if (m_particleModelScale) {
    m_particleModelScale->PutTo(data["ParticleModelScale"]);
  }

  if (m_particleModelColor) {
    m_particleModelColor->PutTo(data["ParticleModelColor"]);
  }

  if (m_particleVelocity1) {
    m_particleVelocity1->PutTo(data["ParticleVelocity1"]);
  }

  if (m_particleVelocity2) {
    m_particleVelocity2->PutTo(data["ParticleVelocity2"]);
  }

  if (m_particleVelocity3) {
    m_particleVelocity3->PutTo(data["ParticleVelocity3"]);
  }

  if (m_particleVelocity4) {
    m_particleVelocity4->PutTo(data["ParticleVelocity4"]);
  }

  if (m_countedChildSystem) {
    m_countedChildSystem->PutTo(data["CountedChildSystem"], Particle::ResourceType());
  }

  if (m_childSystemSpawnCount) {
    m_childSystemSpawnCount->PutTo(data["ChildSystemSpawnCount"]);
  }

  if (m_childSystemSpawnFrame) {
    m_childSystemSpawnFrame->PutTo(data["ChildSystemSpawnFrame"]);
  }

  if (m_doneChildSystem) {
    m_doneChildSystem->PutTo(data["DoneChildSystem"], Particle::ResourceType());
  }

  if (m_doneChildSystemSpawnCount) {
    m_doneChildSystemSpawnCount->PutTo(data["DoneChildSystemSpawnCount"]);
  }

  if (m_doneChildSystemSpawnFrame) {
    m_doneChildSystemSpawnFrame->PutTo(data["DoneChildSystemSpawnFrame"]);
  }

  if (m_intervalChildSystem) {
    m_intervalChildSystem->PutTo(data["IntervalChildSystem"], Particle::ResourceType());
  }

  if (m_intervalChildSystemSpawnInterval) {
    m_intervalChildSystemSpawnInterval->PutTo(data["IntervalChildSystemSpawnInterval"]);
  }

  if (m_intervalChildSystemSpawnFrame) {
    m_intervalChildSystemSpawnFrame->PutTo(data["IntervalChildSystemSpawnFrame"]);
  }

  if (m_spawnSystems) {
    m_spawnSystems->PutTo(data["SpawnSystems"]);
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

  return false && !writer.hasError();
}

void Particle::loadParticleProperties(athena::io::IStreamReader& reader) {
  auto classId = particles::ParticleDataFactory::GetClassID(reader);

  while (classId != FOURCC('_END') && !reader.hasError()) {
    switch (classId.num) {
    case SBIG('PSLT'):
      m_particleSystemLifetime.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('PSWT'):
      m_particleSystemWarmupTime.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('PSTS'):
      m_particleSystemTimeScale.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('POFS'):
      m_particleSystemOffset.reset(particles::ParticleDataFactory::GetVectorElement(reader));
      break;
    case SBIG('SEED'):
      m_particleSystemSeed.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('LENG'):
      m_lineLength.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('WIDT'):
      m_lineWidth.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('MAXP'):
      m_maxParticles.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('GRTE'):
      m_generationRate.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('COLR'):
      m_particleColor.reset(particles::ParticleDataFactory::GetColorElement(reader));
      break;
    case SBIG('LTME'):
      m_particleLifetime.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('EMTR'):
      m_emitter.reset(particles::ParticleDataFactory::GetEmitterElement(reader));
      break;
    case SBIG('LINE'):
      m_lines = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('FXLL'):
      m_scaleLeadingLinePoint = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('AAPH'):
      m_additiveAlpha = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('ZBUF'):
      m_enableZBuffer = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('SORT'):
      m_sort = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('LIT_'):
      m_enableLighting = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('ORNT'):
      m_orientToOrigin = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('RSOP'):
      m_rightVectorScaledOnParticle = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('MBLR'):
      m_motionBlur = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('PMAB'):
      m_particleModelAdditiveAlpha = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('PMUS'):
      m_particleModelUnorientedSquare = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('PMOO'):
      m_particleModelOrientation = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('VMD1'):
      m_vectorMod1Local = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('VMD2'):
      m_vectorMod2Local = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('VMD3'):
      m_vectorMod3Local = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('VMD4'):
      m_vectorMod4Local = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('CIND'):
      m_colorIndirect = particles::ParticleDataFactory::GetInt(reader);
      break;
    case SBIG('OPTS'):
      m_optionalSystem = particles::ParticleDataFactory::GetBool(reader);
      break;
    case SBIG('MBSP'):
      m_motionBlurSamples.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('SIZE'):
      m_particleSize.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ROTA'):
      m_particleAngle.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('TEXR'):
      m_texture.reset(particles::ParticleDataFactory::GetUVElement(reader));
      break;
    case SBIG('TIND'):
      m_indirectTexture.reset(particles::ParticleDataFactory::GetUVElement(reader));
      break;
    case SBIG('PMDL'):
      m_particleModel = particles::ParticleDataFactory::GetAssetID32Big(reader, Model::ResourceType());
      break;
    case SBIG('PMOP'):
      m_particleModelOffset.reset(particles::ParticleDataFactory::GetVectorElement(reader));
      break;
    case SBIG('PMRT'):
      m_particleModelRotation.reset(particles::ParticleDataFactory::GetVectorElement(reader));
      break;
    case SBIG('PMSC'):
      m_particleModelScale.reset(particles::ParticleDataFactory::GetVectorElement(reader));
      break;
    case SBIG('PMCL'):
      m_particleModelColor.reset(particles::ParticleDataFactory::GetColorElement(reader));
      break;
    case SBIG('VEL1'):
      m_particleVelocity1.reset(particles::ParticleDataFactory::GetModVectorElement(reader));
      break;
    case SBIG('VEL2'):
      m_particleVelocity2.reset(particles::ParticleDataFactory::GetModVectorElement(reader));
      break;
    case SBIG('VEL3'):
      m_particleVelocity3.reset(particles::ParticleDataFactory::GetModVectorElement(reader));
      break;
    case SBIG('VEL4'):
      m_particleVelocity4.reset(particles::ParticleDataFactory::GetModVectorElement(reader));
      break;
    case SBIG('ICTS'):
      m_countedChildSystem = particles::ParticleDataFactory::GetAssetID32Big(reader, Particle::ResourceType());
      break;
    case SBIG('NCSY'):
      m_childSystemSpawnCount.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('CSSD'):
      m_childSystemSpawnFrame.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('IDTS'):
      m_doneChildSystem = particles::ParticleDataFactory::GetAssetID32Big(reader, Particle::ResourceType());
      break;
    case SBIG('NDSY'):
      m_doneChildSystemSpawnCount.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('IITS'):
      m_intervalChildSystem = particles::ParticleDataFactory::GetAssetID32Big(reader, Particle::ResourceType());
      break;
    case SBIG('PISY'):
      m_intervalChildSystemSpawnInterval.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('SISY'):
      m_intervalChildSystemSpawnFrame.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('KSSM'):
      if (particles::ParticleDataFactory::GetClassID(reader) != FOURCC('NONE')) {
        m_spawnSystems = particles::SpawnSystemKeyframeData(reader);
      }
      break;
    case SBIG('SSWH'):
      m_childSwooshSystem = particles::ParticleDataFactory::GetAssetID32Big(reader, ParticleSwoosh::ResourceType());
      break;
    case SBIG('SSSD'):
      m_childSwooshSystemSpawnFrame.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('SSPO'):
      m_childSwooshSystemOffset.reset(particles::ParticleDataFactory::GetVectorElement(reader));
      break;
    case SBIG('SELC'):
      m_childElectricSystem = particles::ParticleDataFactory::GetAssetID32Big(reader, ParticleElectric::ResourceType());
      break;
    case SBIG('SESD'):
      m_childElectricSystemSpawnFrame.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('SEPO'):
      m_childElectricSystemOffset.reset(particles::ParticleDataFactory::GetVectorElement(reader));
      break;
    case SBIG('LTYP'):
      m_lightType.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('LCLR'):
      m_lightColor.reset(particles::ParticleDataFactory::GetColorElement(reader));
      break;
    case SBIG('LINT'):
      m_lightIntensity.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('LOFF'):
      m_lightOffset.reset(particles::ParticleDataFactory::GetVectorElement(reader));
      break;
    case SBIG('LDIR'):
      m_lightDirection.reset(particles::ParticleDataFactory::GetVectorElement(reader));
      break;
    case SBIG('LFOT'):
      m_lightFallOffType.reset(particles::ParticleDataFactory::GetIntElement(reader));
      break;
    case SBIG('LFOR'):
      m_lightFallOffRadius.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('LSLA'):
      m_lightCutOffAngle.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ADV1'):
      m_accessParameter1.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ADV2'):
      m_accessParameter2.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ADV3'):
      m_accessParameter3.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ADV4'):
      m_accessParameter4.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ADV5'):
      m_accessParameter5.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ADV6'):
      m_accessParameter6.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ADV7'):
      m_accessParameter7.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    case SBIG('ADV8'):
      m_accessParameter8.reset(particles::ParticleDataFactory::GetRealElement(reader));
      break;
    default:
      std::cout << "Unhandled particle type" << classId.toString() << std::endl;
      break;
    }

    classId = particles::ParticleDataFactory::GetClassID(reader);
  }
}

} // namespace axdl::primedep::MetroidPrime