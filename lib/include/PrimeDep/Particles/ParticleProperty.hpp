#pragma once
#include "../../../../cmake-build-release/_deps/nlohmann-src/include/nlohmann/thirdparty/hedley/hedley.hpp"
#include "ParticleDataFactory.hpp"
#include "PrimeDep/FourCC.hpp"
#include "PrimeDep/Particles/ColorElement.hpp"
#include "PrimeDep/Particles/EmitterElement.hpp"
#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ModVectorElement.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include "PrimeDep/Particles/UVElement.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"
#include "SpawnSystemKeyframeData.hpp"

#include <nlohmann/json_fwd.hpp>
#include <string_view>

namespace athena::io {
class IStreamWriter;
} // namespace athena::io
namespace axdl::primedep::particles {
class IParticleProperty;
class IPropertyContainer {
public:
  void addProperty(IParticleProperty* particleProperty);

  IParticleProperty* propertyForClass(const FourCC& id);
  IParticleProperty* propertyForName(std::string_view name);

  void sortProperties();

protected:
  std::vector<IParticleProperty*> m_properties;
};
class IParticleProperty {
public:
  IParticleProperty(const FourCC& propertyId, const std::string_view propertyName, IPropertyContainer* parent)
  : m_propertyId(propertyId), m_propertyName(propertyName) {
    parent->addProperty(this);
  }
  virtual ~IParticleProperty() = default;

  const FourCC& propertyId() const { return m_propertyId; }
  std::string_view propertyName() const { return m_propertyName; }

  virtual void PutTo(athena::io::IStreamWriter& writer) const = 0;
  virtual void PutTo(nlohmann::ordered_json& writer) const = 0;

  virtual void loadValue(athena::io::IStreamReader& reader) = 0;
  virtual void loadValue(const nlohmann::ordered_json& reader) = 0;

  int loadOrder() const { return m_loadOrder; }
  void setLoadOrder(const int loadOrder) { m_loadOrder = loadOrder; }

protected:
  FourCC m_propertyId;
  std::string m_propertyName;
  int m_loadOrder = -1;
};

class ElementParticleProperty : public IParticleProperty {
public:
  ElementParticleProperty(const FourCC& propertyId, const std::string_view propertyName, IPropertyContainer* parent)
  : IParticleProperty(propertyId, propertyName, parent) {}
  void PutTo(athena::io::IStreamWriter& writer) const override {
    if (m_element) {
      m_element->PutTo(writer);
    }
  }

  void PutTo(nlohmann::ordered_json& writer) const override {
    if (m_element) {
      m_element->PutTo(writer);
    }
  }

protected:
  std::unique_ptr<IElement> m_element{};
};

template <class ElementType>
class TypedElementProperty final : public ElementParticleProperty {
public:
  TypedElementProperty(const FourCC& propertyId, const std::string& propertyName, IPropertyContainer* parent)
  : ElementParticleProperty(propertyId, propertyName, parent) {}
  void loadValue(athena::io::IStreamReader& reader) override {
    m_element.reset(ParticleDataFactory::GetElement<ElementType>(reader, m_propertyName));
    if (m_element) {
      m_element->setPropertyId(m_propertyId);
    }
  }
  void loadValue(const nlohmann::ordered_json& reader) override {
    m_element.reset(ParticleDataFactory::GetElement<ElementType>(reader, m_propertyName));
    if (m_element) {
      m_element->setPropertyId(m_propertyId);
    }
  }

  ElementType* elementTyped() const { return m_element.get(); }
};

class BoolElementProperty final : public IParticleProperty {
public:
  BoolElementProperty(bool defaultValue, const FourCC& propertyId, std::string_view propertyName,
                      IPropertyContainer* parent);

  void loadValue(athena::io::IStreamReader& reader) override;
  void loadValue(const nlohmann::ordered_json& reader) override;

  void PutTo(athena::io::IStreamWriter& writer) const override;
  void PutTo(nlohmann::ordered_json& writer) const override;

private:
  std::optional<bool> m_value;
  bool m_defaultValue;
};

class AssetID32BigElementProperty final : public IParticleProperty {
public:
  AssetID32BigElementProperty(const FourCC& assetType, const FourCC& propertyId, std::string_view propertyName,
                              IPropertyContainer* parent)
  : IParticleProperty(propertyId, propertyName, parent), m_type(assetType) {}

  void loadValue(athena::io::IStreamReader& reader) override;
  void loadValue(const nlohmann::ordered_json& reader) override;

  void PutTo(athena::io::IStreamWriter& writer) const override;
  void PutTo(nlohmann::ordered_json& writer) const override;

private:
  FourCC m_type;
  std::optional<AssetId32Big> m_value;
};

class SpawnSystemKeyframeDataElementProperty final : public IParticleProperty {
public:
  SpawnSystemKeyframeDataElementProperty(const FourCC& propertyId, std::string_view propertyName,
                                         IPropertyContainer* parent)
  : IParticleProperty(propertyId, propertyName, parent) {}

  void loadValue(athena::io::IStreamReader& reader) override;
  void loadValue(const nlohmann::ordered_json& reader) override;

  void PutTo(athena::io::IStreamWriter& writer) const override;
  void PutTo(nlohmann::ordered_json& writer) const override;

private:
  std::optional<SpawnSystemKeyframeData> m_value;
};

inline void IPropertyContainer::sortProperties() {
  std::ranges::sort(m_properties, [](const IParticleProperty* lhs, const IParticleProperty* rhs) {
    return lhs->loadOrder() < rhs->loadOrder();
  });
}

using IntElementProperty = TypedElementProperty<IntElement>;
using RealElementProperty = TypedElementProperty<RealElement>;
using ColorElementProperty = TypedElementProperty<ColorElement>;
using VectorElementProperty = TypedElementProperty<VectorElement>;
using ModVectorElementProperty = TypedElementProperty<ModVectorElement>;
using EmitterElementProperty = TypedElementProperty<EmitterElement>;
using UVElementProperty = TypedElementProperty<UVElement>;

} // namespace axdl::primedep::particles