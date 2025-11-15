#include "PrimeDep/Particles/ParticleProperty.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
void IPropertyContainer::addProperty(IParticleProperty* particleProperty) {
  if (!particleProperty || propertyForClass(particleProperty->propertyId()) ||
      propertyForName(particleProperty->propertyName())) {
    return;
  }

  m_properties.emplace_back(particleProperty);
}

IParticleProperty* IPropertyContainer::propertyForClass(const FourCC& id) {
  const auto it = std::ranges::find_if(m_properties, [&id](const auto& elem) { return elem->propertyId() == id; });
  if (it == m_properties.end()) {
    return nullptr;
  }
  return *it;
}

IParticleProperty* IPropertyContainer::propertyForName(const std::string_view name) {
  const auto it =
      std::ranges::find_if(m_properties, [&name](const auto& elem) { return elem->propertyName() == name; });
  if (it == m_properties.end()) {
    return nullptr;
  }
  return *it;
}

BoolElementProperty::BoolElementProperty(const bool defaultValue, const FourCC& propertyId,
                                         const std::string_view propertyName, IPropertyContainer* parent)
: IParticleProperty(propertyId, propertyName, parent), m_defaultValue(defaultValue) {}

void BoolElementProperty::loadValue(athena::io::IStreamReader& reader) {
  // We don't need to worry about the class ID because the reader already takes care if it
  m_value = ParticleDataFactory::GetBool(reader, m_defaultValue);
}

void BoolElementProperty::loadValue(const nlohmann::ordered_json& reader) {
  m_value = reader.value(m_propertyName, m_defaultValue);
}

void BoolElementProperty::PutTo(athena::io::IStreamWriter& writer) const {
  if (m_value) {
    ParticleDataFactory::SetClassID(writer, m_propertyId);
    ParticleDataFactory::SetBool(writer, *m_value);
  }
}

void BoolElementProperty::PutTo(nlohmann::ordered_json& writer) const {
  if (!m_value) {
    return;
  }
  writer[propertyName()] = m_value;
}

void AssetID32BigElementProperty::loadValue(athena::io::IStreamReader& reader) {
  // We don't need to worry about the class ID because the reader already takes care if it
  m_value = ParticleDataFactory::GetAssetID32Big(reader, m_type);
}

void AssetID32BigElementProperty::loadValue(const nlohmann::ordered_json& reader) {
  m_value = AssetId32Big(reader, m_type);
}

void AssetID32BigElementProperty::PutTo(athena::io::IStreamWriter& writer) const {
  ParticleDataFactory::SetClassID(writer, m_propertyId);
  if (m_value && *m_value == kInvalidAssetId32Big) {
    ParticleDataFactory::SetClassID(writer, FOURCC('NONE'));
  } else if (m_value) {
    ParticleDataFactory::SetClassID(writer, FOURCC('CNST'));
    m_value->PutTo(writer);
  }
}

void AssetID32BigElementProperty::PutTo(nlohmann::ordered_json& writer) const {
  if (m_value) {
    m_value->PutTo(writer[propertyName()], m_type);
  }
}

void SpawnSystemKeyframeDataElementProperty::loadValue(athena::io::IStreamReader& reader) {
  if (ParticleDataFactory::GetClassID(reader) == FOURCC('NONE')) {
    return;
  }

  m_value = SpawnSystemKeyframeData(reader);
}

void SpawnSystemKeyframeDataElementProperty::loadValue(const nlohmann::ordered_json& reader) {
  if (reader.is_null() || !reader.is_object()) {
    return;
  }
  m_value = SpawnSystemKeyframeData(reader);
}

void SpawnSystemKeyframeDataElementProperty::PutTo(athena::io::IStreamWriter& writer) const {
  ParticleDataFactory::SetClassID(writer, m_propertyId);
  if (!m_value) {
    ParticleDataFactory::SetClassID(writer, FOURCC('NONE'));
    return;
  }
  ParticleDataFactory::SetClassID(writer, FOURCC('CNST'));
  m_value->PutTo(writer);
}

void SpawnSystemKeyframeDataElementProperty::PutTo(nlohmann::ordered_json& writer) const {
  if (!m_value) {
    writer[propertyName()] = nullptr;
    return;
  }
  m_value->PutTo(writer[propertyName()]);
}

} // namespace axdl::primedep::particles