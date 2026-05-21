#pragma once

#include "PrimeDep/IResource.hpp"

#include "PrimeDep/Particles/ParticleProperty.hpp"

namespace axdl::primedep::MetroidPrime {

class DecalData final : public TypedResource('DPSC', ".dpsm", ".dpsm.dpsc", DESCRIPTION("Decal Particle System")),
                        particles::IPropertyContainer {
public:
  template <char Num>
  struct QuadDescription {
    QuadDescription(IPropertyContainer* parent)
    : m_lifetime(FOURCCByChar(Num, 'L', 'F', 'T'), std::format("Quad{}Lifetime", Num), parent)
    , m_size(FOURCCByChar(Num, 'S', 'Z', 'E'), std::format("Quad{}Size", Num), parent)
    , m_rotation(FOURCCByChar(Num, 'R', 'O', 'T'), std::format("Quad{}Rotation", Num) + "Rotation", parent)
    , m_offset(FOURCCByChar(Num, 'O', 'F', 'F'), std::format("Quad{}Offset", Num) + "Offset", parent)
    , m_color(FOURCCByChar(Num, 'C', 'L', 'R'), std::format("Quad{}Color", Num), parent)
    , m_texture(FOURCCByChar(Num, 'T', 'E', 'X'), std::format("Quad{}Texture", Num), parent)
    , m_additiveBlend(false, FOURCCByChar(Num, 'A', 'D', 'D'), std::format("Quad{}AdditiveBlend", Num), parent) {}
    particles::IntElementProperty m_lifetime;
    particles::RealElementProperty m_size;
    particles::RealElementProperty m_rotation;
    particles::VectorElementProperty m_offset;
    particles::ColorElementProperty m_color;
    particles::UVElementProperty m_texture;
    particles::BoolElementProperty m_additiveBlend;
  };
  DecalData();
  DecalData(const char* ptr, std::size_t size);
  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  bool writeUncooked(const std::string_view path) const override;
  
  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }

private:
  QuadDescription<'1'> m_quad1;
  QuadDescription<'2'> m_quad2;
  particles::AssetID32BigElementProperty m_model;
  particles::IntElementProperty m_lifetime;
  particles::VectorElementProperty m_modelOffset;
  particles::VectorElementProperty m_modelRotation;
  particles::VectorElementProperty m_modelScale;
  particles::ColorElementProperty m_modelColor;
  particles::BoolElementProperty m_modelAdditiveBlend;
  particles::BoolElementProperty m_modelRotationGlobal;
};
} // namespace axdl::primedep::MetroidPrime