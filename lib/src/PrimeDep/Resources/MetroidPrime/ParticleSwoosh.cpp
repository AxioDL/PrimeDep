#include "PrimeDep/Resources/MetroidPrime/ParticleSwoosh.hpp"

#include "athena/FileReader.hpp"
#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime {
ParticleSwoosh::ParticleSwoosh()
: m_particleSystemLifetime{FOURCC('PSLT'), "ParticleSystemLifetime", this}
, m_timeMultiplier{FOURCC('TIME'), "TimeMultiplier", this}
, m_leftRadius{FOURCC('LRAD'), "LeftRadius", this}
, m_rightRadius{FOURCC('RRAD'), "RightRadius", this}
, m_length{FOURCC('LENG'), "Length", this}
, m_color{FOURCC('COLR'), "Color", this}
, m_side{FOURCC('SIDE'), "Side", this}
, m_initialRotation{FOURCC('IROT'), "InitialRotation", this}
, m_masterRotation{FOURCC('ROTM'), "MasterRotation", this}
, m_particleOffset{FOURCC('POFS'), "ParticleOffset", this}
, m_initialVelocity{FOURCC('IVEL'), "InitialVelocity", this}
, m_nextPosition{FOURCC('NPOS'), "NextPosition", this}
, m_velocity1{FOURCC('VELM'), "Velocity1", this}
, m_velocity2{FOURCC('VLM2'), "Velocity2", this}
, m_splineSegments{FOURCC('SPLN'), "SplineSegments", this}
, m_texture{FOURCC('TEXR'), "Texture", this}
, m_textureSpan{FOURCC('TSPN'), "TextureSpan", this}
, m_leftRadiusForRight{false, FOURCC('LLRD'), "LeftRadiusForRight", this}
, m_crossSection{false, FOURCC('CROS'), "CrossSection", this}
, m_localVelocity1{false, FOURCC('VLS1'), "LocalVelocity1", this}
, m_localVelocity2{false, FOURCC('VLS2'), "LocalVelocity2", this}
, m_singleRotation{false, FOURCC('SROT'), "SingleRotation", this}
, m_wireframe{false, FOURCC('WIRE'), "Wireframe", this}
, m_wireframeTexture{FOURCC('TEXW'), "LocalVelocity1", this}
, m_additiveAlpha{false, FOURCC('AALP'), "AdditiveAlpha", this}
, m_enableZBuffer{false, FOURCC('ZBUF'), "EnableZBuffer", this}
, m_viewOrient{false, FOURCC('ORNT'), "ViewOrient", this}
, m_useClockRandom{false, FOURCC('CRND'), "UseClockRandom", this} {}

ParticleSwoosh::ParticleSwoosh(const char* ptr, const std::size_t size) : ParticleSwoosh() {
  athena::io::MemoryReader mr(ptr, size, true);

  if (particles::ParticleDataFactory::GetClassID(mr) != FOURCC('SWSH')) {
    return;
  }

  auto clsId = particles::ParticleDataFactory::GetClassID(mr);
  int loadOrder = 0;
  while (clsId != FOURCC('_END')) {
    if (const auto& prop = propertyForClass(clsId)) {
      prop->loadValue(mr);
      prop->setLoadOrder(loadOrder++);
    }

    clsId = particles::ParticleDataFactory::GetClassID(mr);
  }

  sortProperties();
}

ParticleSwoosh::ParticleSwoosh(const nlohmann::ordered_json& in) : ParticleSwoosh() {
  for (int loadOrder = 0; const auto& [key, _] : in.items()) {
    const auto property = propertyForName(key);
    if (!property) {
      continue;
    }

    property->loadValue(in);
    property->setLoadOrder(loadOrder++);
  }

  sortProperties();
}

bool ParticleSwoosh::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter writer(p.generic_string());
  particles::ParticleDataFactory::SetClassID(writer, FOURCC('SWSH'));

  for (const auto& prop : m_properties) {
    if (!prop || prop->loadOrder() == -1) {
      continue;
    }
    prop->PutTo(writer);
  }

  particles::ParticleDataFactory::SetClassID(writer, FOURCC('_END'));

  return !writer.hasError();
}

bool ParticleSwoosh::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);

  nlohmann::ordered_json out = nlohmann::ordered_json::object();
  for (const auto& property : m_properties) {
    if (property->loadOrder() == -1) {
      continue;
    }
    property->PutTo(out);
  }

  athena::io::FileWriter writer(p.generic_string());
  const std::string json = out.dump(4) + "\n";
  writer.writeString(json, json.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> ParticleSwoosh::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<ParticleSwoosh>(ptr, size);
}

std::shared_ptr<IResource> ParticleSwoosh::ingest(const nlohmann::ordered_json& metadata, const std::string_view path) {
  const auto p = GetRawPath(path);
  athena::io::FileReader reader(p.generic_string());
  nlohmann::ordered_json in = nlohmann::ordered_json::parse(reader.readString());
  return std::make_shared<ParticleSwoosh>(in);
}

} // namespace axdl::primedep::MetroidPrime