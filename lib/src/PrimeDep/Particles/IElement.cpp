#include "PrimeDep/Particles/IElement.hpp"

#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
void IElement::PutTo(athena::io::IStreamWriter& out) const {
  if (!isValid()) {
    return;
  }
  if (m_propertyId != kInvalidFourCC) {
    ParticleDataFactory::SetClassID(out, m_propertyId);
  }
  if (classId() != kInvalidFourCC) {
    ParticleDataFactory::SetClassID(out, classId());
  }
  PutToInternal(out);
}

void IElement::PutTo(nlohmann::ordered_json& out) const {
  if (!isValid()) {
    return;
  }
  auto& value = m_propertyName.empty() ? out : out[m_propertyName.c_str()];
  if (!className().empty()) {
    ParticleDataFactory::SetClassID(value, className());
  }
  PutToInternal(value);
}

BoolElement::BoolElement(const bool value, athena::io::IStreamReader& reader) : IElement(reader), m_value(value) {}
BoolElement::BoolElement(const bool value, const nlohmann::ordered_json& reader) : IElement(reader), m_value(value) {}

void BoolElement::PutToInternal(athena::io::IStreamWriter& out) const { ParticleDataFactory::SetBool(out, m_value); }
void BoolElement::PutToInternal(nlohmann::ordered_json& out) const { out = m_value; }

} // namespace axdl::primedep::particles