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

uint32_t GXGetTexBufferSize(uint16_t width, uint16_t height, ETexelFormat fmt, bool mips, uint8_t maxLod) {
  int32_t shiftX = 0;
  int32_t shiftY = 0;
  switch (fmt) {
  case ETexelFormat::I4:
  case ETexelFormat::C4:
  case ETexelFormat::CMPR:
    shiftX = 3;
    shiftY = 3;
    break;
  case ETexelFormat::I8:
  case ETexelFormat::IA4:
  case ETexelFormat::C8:
    shiftX = 3;
    shiftY = 2;
    break;
  case ETexelFormat::IA8:
  case ETexelFormat::C14X2:
  case ETexelFormat::RGB565:
  case ETexelFormat::RGB5A3:
    shiftX = 2;
    shiftY = 2;
    break;
  default:
    break;
  }
  const uint32_t bitSize = fmt == ETexelFormat::RGBA8 ? 64 : 32;
  uint32_t bufLen = 0;
  if (mips) {
    while (maxLod != 0) {
      const uint32_t tileX = ((width + (1 << shiftX) - 1) >> shiftX);
      const uint32_t tileY = ((height + (1 << shiftY) - 1) >> shiftY);
      bufLen += bitSize * tileX * tileY;

      if (width == 1 && height == 1) {
        return bufLen;
      }

      width = (width < 2) ? 1 : width / 2;
      height = (height < 2) ? 1 : height / 2;
      --maxLod;
    };
  } else {
    const uint32_t tileX = ((width + (1 << shiftX) - 1) >> shiftX);
    const uint32_t tileY = ((height + (1 << shiftY) - 1) >> shiftY);
    bufLen = bitSize * tileX * tileY;
  }

  return bufLen;
}

GraphicsPalette::GraphicsPalette(athena::io::IStreamReader& in) {
  m_format = static_cast<EPaletteFormat>(in.readUint32Big());
  m_width = in.readUint16Big();
  m_height = in.readUint16Big();
  m_entries = std::make_unique<uint16_t[]>(entryCount());
  in.readUBytesToBuf(m_entries.get(), entryCount() * sizeof(uint16_t));
}

void GraphicsPalette::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(static_cast<uint32_t>(m_format));
  out.writeUint16Big(m_width);
  out.writeUint16Big(m_height);
  out.writeBytes(m_entries.get(), entryCount() * sizeof(uint16_t));
}

void GraphicsPalette::PutTo(nlohmann::ordered_json& out) const {
  out["Format"] = magic_enum::enum_name(m_format);
  out["Width"] = m_width;
  out["Height"] = m_height;
}

Texture::Texture(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);

  m_format = static_cast<ETexelFormat>(in.readUint32Big());
  m_width = in.readUint16Big();
  m_height = in.readUint16Big();
  m_numMips = in.readUint32Big();

  if (m_format == ETexelFormat::C4 || m_format == ETexelFormat::C8 || m_format == ETexelFormat::C14X2) {
    m_palette = GraphicsPalette(in);
  }

  const auto readLen = GXGetTexBufferSize(m_width, m_height, m_format, m_numMips > 0, m_numMips);
  m_data = std::make_unique<uint8_t[]>(readLen);
  if (const auto actualSize = in.readUBytesToBuf(m_data.get(), readLen); actualSize != readLen) {
    printf("Failed to read texture data\n");
  }
}

bool Texture::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());
  out.writeUint32Big(static_cast<uint32_t>(m_format));
  out.writeUint16Big(m_width);
  out.writeUint16Big(m_height);
  out.writeUint32Big(m_numMips);
  if (m_palette) {
    m_palette->PutTo(out);
  }
  const auto writeLen = GXGetTexBufferSize(m_width, m_height, m_format, m_numMips > 0, m_numMips);
  out.writeUBytes(m_data.get(), writeLen);

  return !out.hasError();
}

bool Texture::writeUncooked(std::string_view path) const { return false; }

nlohmann::ordered_json Texture::metadata(const std::string_view path) const {
  nlohmann::ordered_json json = ITypedResource::metadata(path);
  json["Format"] = magic_enum::enum_name<ETexelFormat>(format());
  json["Width"] = m_width;
  json["Height"] = m_height;
  json["NumMips"] = m_numMips;
  if (m_palette) {
    m_palette->PutTo(json["GraphicsPalette"]);
  }

  return json;
}

std::shared_ptr<IResource> Texture::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<Texture>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime