#include "PrimeDep/Resources/MetroidPrime/Texture.hpp"

#include <athena/MemoryReader.hpp>
#include <magic_enum/magic_enum.hpp>

namespace axdl::primedep ::MetroidPrime {

constexpr uint32_t TexelFormatBitsPerPixel(const ETexelFormat fmt) {
  switch (fmt) {
  case ETexelFormat::I4:
  case ETexelFormat::C4:
  case ETexelFormat::CMPR:
    return 4;
  case ETexelFormat::I8:
  case ETexelFormat::IA4:
  case ETexelFormat::C8:
    return 8;
  case ETexelFormat::IA8:
  case ETexelFormat::C14X2:
  case ETexelFormat::RGB565:
  case ETexelFormat::RGB5A3:
    return 16;
  case ETexelFormat::RGBA8:
    return 32;
  default:
    return 0;
  }
}

GraphicsPalette::GraphicsPalette(athena::io::IStreamReader& in) {
  m_format = static_cast<EPaletteFormat>(in.readUint32Big());
  m_width = in.readUint16Big();
  m_height = in.readUint16Big();
  m_entries = std::make_unique<uint16_t[]>(entryCount());
  in.readUBytesToBuf(m_entries.get(), entryCount() * sizeof(uint16_t));
}

Texture::Texture(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc) : ITypedResource(desc) {
  athena::io::MemoryReader in(ptr, size, true);

  m_format = static_cast<ETexelFormat>(in.readUint32Big());
  m_width = in.readUint16Big();
  m_height = in.readUint16Big();
  m_numMips = in.readUint32Big();

  if (m_format == ETexelFormat::C4 || m_format == ETexelFormat::C8 || m_format == ETexelFormat::C14X2) {
    m_palette = GraphicsPalette(in);
  }

  // TODO: Do this properly
  const auto readLen = size - in.position();
  m_data = std::make_unique<uint8_t[]>(readLen);
  if (const auto actualSize = in.readUBytesToBuf(m_data.get(), readLen); actualSize != readLen) {
    printf("Failed to read texture data\n");
  }
}

std::shared_ptr<IResource> Texture::loadCooked(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc) {
  return std::make_shared<Texture>(ptr, size, desc);
}

nlohmann::ordered_json Texture::metadata(const std::string_view path) const {
  nlohmann::ordered_json json = ITypedResource::metadata(path);
  json["Format"] = magic_enum::enum_name<ETexelFormat>(format());
  json["Width"] = m_width;
  json["Height"] = m_height;
  json["NumMips"] = m_numMips;

  return json;
}

} // namespace axdl::primedep::MetroidPrime