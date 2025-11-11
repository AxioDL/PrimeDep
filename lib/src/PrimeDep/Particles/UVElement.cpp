#include "PrimeDep/Particles/UVElement.hpp"

#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Resources/MetroidPrime/Texture.hpp"

namespace axdl::primedep::particles {

UVEConstant32Big::UVEConstant32Big(athena::io::IStreamReader& reader)
: UVElement(reader), m_textureId(ParticleDataFactory::GetAssetID32Big(reader, MetroidPrime::Texture::ResourceType())) {}

UVEConstant32Big::UVEConstant32Big(const nlohmann::ordered_json& reader) : UVElement(reader) {
  m_textureId = AssetId32Big(reader, MetroidPrime::Texture::ResourceType());
}
void UVEConstant32Big::PutToInternal(athena::io::IStreamWriter& out) const {
  if (!m_textureId || *m_textureId == kInvalidAssetId32Big) {
    ParticleDataFactory::SetClassID(out, FOURCC('NONE'));
  } else {
    ParticleDataFactory::SetClassID(out, FOURCC('CNST'));
    m_textureId->PutTo(out);
  }
}

void UVEConstant32Big::PutToInternal(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetAssetID32Big(out, "Texture", m_textureId, MetroidPrime::Texture::ResourceType());
}

UVEAnimated32Big::UVEAnimated32Big(athena::io::IStreamReader& reader)
: UVElement(reader)
, m_textureId(ParticleDataFactory::GetAssetID32Big(reader, MetroidPrime::Texture::ResourceType()))
, m_tileWidth(ParticleDataFactory::GetIntElement(reader, "TileWidth"))
, m_tileHeight(ParticleDataFactory::GetIntElement(reader, "TileHeight"))
, m_strideWidth(ParticleDataFactory::GetIntElement(reader, "StrideWidth"))
, m_strideHeight(ParticleDataFactory::GetIntElement(reader, "StrideWidth"))
, m_loop(ParticleDataFactory::GetBool(reader)) {}

UVEAnimated32Big::UVEAnimated32Big(const nlohmann::ordered_json& reader)
: UVElement(reader)
, m_textureId(ParticleDataFactory::GetAssetID32Big(reader, "Texture", MetroidPrime::Texture::ResourceType()))
, m_tileWidth(ParticleDataFactory::GetIntElement(reader, "TileWidth"))
, m_tileHeight(ParticleDataFactory::GetIntElement(reader, "TileHeight"))
, m_strideWidth(ParticleDataFactory::GetIntElement(reader, "StrideWidth"))
, m_strideHeight(ParticleDataFactory::GetIntElement(reader, "StrideWidth"))
, m_loop(ParticleDataFactory::GetBool(reader, "Loop")) {}

UVEAnimated32Big::~UVEAnimated32Big() {}

void UVEAnimated32Big::PutToInternal(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetAssetID32Big(out, m_textureId);
  m_tileWidth->PutTo(out);
  m_tileHeight->PutTo(out);
  m_strideWidth->PutTo(out);
  m_strideHeight->PutTo(out);
  ParticleDataFactory::SetBool(out, m_loop);
}

void UVEAnimated32Big::PutToInternal(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetAssetID32Big(out, "Texture", m_textureId, MetroidPrime::Texture::ResourceType());
  m_tileWidth->PutTo(out);
  m_tileHeight->PutTo(out);
  m_strideWidth->PutTo(out);
  m_strideHeight->PutTo(out);
  ParticleDataFactory::SetBool(out, "Loop", m_loop);
}
}; // namespace axdl::primedep::particles