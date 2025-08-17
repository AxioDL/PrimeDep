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

namespace axdl::primedep::MetroidPrime {
Particle::Particle(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  if (particles::ParticleDataFactory::GetClassID(in) != FOURCC('GPSM')) {
    return;
  }
  // loadParticleProperties(in);
}

std::shared_ptr<IResource> Particle::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<Particle>(ptr, size);
}

[[nodiscard]] bool Particle::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json data;

  if (m_particleSystemLifetime) {
    data["ParticleSystemLifetime"] = m_particleSystemLifetime->value();
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = data.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

[[nodiscard]] bool Particle::writeCooked(const std::string_view path) const { return false; }

void Particle::loadParticleProperties(athena::io::IStreamReader& reader) {
  auto classId = particles::ParticleDataFactory::GetClassID(reader);

  while (classId != FOURCC('_END')) {
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
      m_spawnSystems = particles::SpawnSystemKeyframeData(reader);
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
      break;
    }

    classId = particles::ParticleDataFactory::GetClassID(reader);
  }
}

} // namespace axdl::primedep::MetroidPrime