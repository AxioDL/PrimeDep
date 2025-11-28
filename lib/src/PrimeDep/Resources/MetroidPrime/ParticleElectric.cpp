#include "PrimeDep/Resources/MetroidPrime/ParticleElectric.hpp"

#include "PrimeDep/ResourcePool.hpp"
#include "PrimeDep/Resources/MetroidPrime/Particle.hpp"
#include "PrimeDep/Resources/MetroidPrime/ParticleSwoosh.hpp"
#include "athena/FileReader.hpp"
#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime {
ParticleElectric::ParticleElectric()
: m_particleSystemLifetime{FOURCC('LIFE'), "ParticleSystemLifetime", this}
, m_segmentLifetime{FOURCC('SLIF'), "SegmentLifetime", this}
, m_generationRate{FOURCC('GRAT'), "GenerationRate", this}
, m_instanceCount{FOURCC('SCNT'), "InstanceCount", this}
, m_segmentCount{FOURCC('SSEG'), "SegmentCount", this}
, m_color{FOURCC('COLR'), "Color", this}
, m_initialEmitter{FOURCC('IEMT'), "InitialEmitter", this}
, m_finalEmitter{FOURCC('FEMT'), "FinalEmitter", this}
, m_amplitudeLength{FOURCC('AMPL'), "AmplitudeLength", this}
, m_amplitudeDirect{FOURCC('AMPD'), "AmplitudeDirect", this}
, m_lineWidth1{FOURCC('LWD1'), "LineWidth1", this}
, m_lineWidth2{FOURCC('LWD2'), "LineWidth2", this}
, m_lineWidth3{FOURCC('LWD3'), "LineWidth3", this}
, m_lineColor1{FOURCC('LCL1'), "LineColor1", this}
, m_lineColor2{FOURCC('LCL2'), "LineColor2", this}
, m_lineColor3{FOURCC('LCL3'), "LineColor3", this}
, m_childSwoosh{ParticleSwoosh::ResourceType(), FOURCC('SSWH'), "ChildSwoosh", this}
, m_startParticle{Particle::ResourceType(), FOURCC('GPSM'), "StartParticle", this}
, m_endParticle{Particle::ResourceType(), FOURCC('EPSM'), "EndParticle", this}
, m_zeroY{false, FOURCC('ZERY'), "ZeroY", this} {}

ParticleElectric::ParticleElectric(const char* ptr, const std::size_t size) : ParticleElectric() {
  athena::io::MemoryReader mr(ptr, size, true);
  if (particles::ParticleDataFactory::GetClassID(mr) != FOURCC('ELSM')) {
    return;
  }

  int loadOrder = 0;
  auto clsId = particles::ParticleDataFactory::GetClassID(mr);
  while (clsId != FOURCC('_END')) {
    if (const auto property = propertyForClass(clsId)) {
      property->loadValue(mr);
      property->setLoadOrder(loadOrder++);
    }

    clsId = particles::ParticleDataFactory::GetClassID(mr);
  }

  sortProperties();
}

ParticleElectric::ParticleElectric(const nlohmann::ordered_json& in) : ParticleElectric() {
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

bool ParticleElectric::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter writer(p.generic_string());
  particles::ParticleDataFactory::SetClassID(writer, FOURCC('ELSM'));

  for (const auto& property : m_properties) {
    if (property->loadOrder() == -1) {
      continue;
    }
    property->PutTo(writer);
  }

  particles::ParticleDataFactory::SetClassID(writer, FOURCC('_END'));

  return !writer.hasError();
}

std::shared_ptr<IResource> ParticleElectric::ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
  const auto p = GetRawPath(path);
  athena::io::FileReader reader(p.generic_string());
  nlohmann::ordered_json in = nlohmann::ordered_json::parse(reader.readString());
  return std::make_shared<ParticleElectric>(in);
}

bool ParticleElectric::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);

  nlohmann::ordered_json out = nlohmann::json::object();

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

std::shared_ptr<IResource> ParticleElectric::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<ParticleElectric>(ptr, size);
}

uint32_t ParticleElectric::immediateChildCount() const {
  return !!m_childSwoosh.value() + !!m_startParticle.value() + !!m_endParticle.value();
}

std::optional<std::vector<std::shared_ptr<IResource>>> ParticleElectric::immediateChildren() const {
  std::vector<std::shared_ptr<IResource>> children;

  if (m_childSwoosh.value()) {
    auto res = ResourcePool32Big::instance()->resourceById(m_childSwoosh.asObjectTag());
    if (!res) {
      res = ResourcePool32Big::instance()->ingestResourceByRepPath(m_childSwoosh.value()->repPath(),
                                                                   m_childSwoosh.type());
    }
    if (res) {
      children.emplace_back(res);
    }
  }

  if (m_startParticle.value()) {
    auto res = ResourcePool32Big::instance()->resourceById(m_startParticle.asObjectTag());
    if (!res) {
      res = ResourcePool32Big::instance()->ingestResourceByRepPath(m_startParticle.value()->repPath(),
                                                                   m_startParticle.type());
    }
    if (res) {
      children.emplace_back(res);
    }
  }

  if (m_endParticle.value()) {
    auto res = ResourcePool32Big::instance()->resourceById(m_endParticle.asObjectTag());
    if (!res) {
      res = ResourcePool32Big::instance()->ingestResourceByRepPath(m_endParticle.value()->repPath(),
                                                                   m_endParticle.type());
    }
    if (res) {
      children.emplace_back(res);
    }
  }

  if (children.empty()) {
    return std::nullopt;
  }

  return {children};
}

std::optional<std::vector<ObjectTag32Big>> ParticleElectric::allChildTags() const {
  std::vector<ObjectTag32Big> children;

  if (const auto childResources = immediateChildren()) {
    for (const auto& res : *childResources) {
      if (!res) {
        continue;
      }
      auto tags = res->allChildTags();
      if (!tags) {
        // Add the child resource's children
        children.insert(children.end(), tags->begin(), tags->end());
        continue;
      }
      // We also need to add this resource
      children.emplace_back(res->typeCode(), res->assetId32Big());
    }
  }

  if (children.empty()) {
    return std::nullopt;
  }
  return {children};
}

} // namespace axdl::primedep::MetroidPrime