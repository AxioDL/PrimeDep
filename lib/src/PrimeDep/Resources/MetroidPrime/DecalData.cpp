#include "PrimeDep/Resources/MetroidPrime/DecalData.hpp"

#include "athena/MemoryReader.hpp"

#include <iostream>

namespace axdl::primedep::MetroidPrime {
DecalData::DecalData()
: m_quad1(this)
, m_quad2(this)
, m_model(FOURCC('CMDL'), FOURCC('DMDL'), "Model", this)
, m_lifetime(FOURCC('DLFT'), "Lifetime", this)
, m_modelOffset(FOURCC('DMOP'), "ModelOffset", this)
, m_modelRotation(FOURCC('DMRT'), "ModelRotation", this)
, m_modelScale(FOURCC('DMSC'), "ModelScale", this)
, m_modelColor(FOURCC('DMCL'), "ModelColor", this)
, m_modelAdditiveBlend(false, FOURCC('DMAB'), "ModelAdditiveBlend", this)
, m_modelRotationGlobal(false, FOURCC('DMOO'), "ModelRotationGlobal", this) {}

DecalData::DecalData(const char* ptr, const std::size_t size) : DecalData() {
  athena::io::MemoryReader in(ptr, size, true);

  if (particles::ParticleDataFactory::GetClassID(in) != FOURCC('DPSM')) {
    return;
  }

  auto classId = particles::ParticleDataFactory::GetClassID(in);

  int loadOrder = 0;
  while (classId != FOURCC('_END') && !in.hasError()) {
    if (auto* cls = propertyForClass(classId)) {
      cls->loadValue(in);
      cls->setLoadOrder(loadOrder);
      ++loadOrder;
    } else {
      std::cout << "Unhandled class " << classId.toString() << std::endl;
    }
    classId = particles::ParticleDataFactory::GetClassID(in);
  }

  sortProperties();
}

[[nodiscard]] bool DecalData::writeUncooked(const std::string_view path) const {
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

std::shared_ptr<IResource> DecalData::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<DecalData>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime