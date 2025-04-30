#include "PrimeDep/Resources/MetroidPrime/Texture.hpp"
#include "PrimeDep/texture_decode.hpp"

#include <athena/MemoryReader.hpp>
#include <magic_enum/magic_enum.hpp>

#include <png.h>

namespace axdl::primedep ::MetroidPrime {

constexpr uint32_t RetroFormatToGXFormat(const ETexelFormat fmt) {
  switch (fmt) {
  case ETexelFormat::C4:
    return GX_TF_C4;
  case ETexelFormat::C8:
    return GX_TF_C8;
  case ETexelFormat::C14X2:
    return GX_TF_C14X2;
  case ETexelFormat::I4:
    return GX_TF_I4;
  case ETexelFormat::I8:
    return GX_TF_I8;
  case ETexelFormat::IA4:
    return GX_TF_IA4;
  case ETexelFormat::IA8:
    return GX_TF_IA8;
  case ETexelFormat::RGB565:
    return GX_TF_RGB565;
  case ETexelFormat::RGB5A3:
    return GX_TF_RGB5A3;
  case ETexelFormat::RGBA8:
    return GX_TF_RGBA8;
  case ETexelFormat::CMPR:
    return GX_TF_CMPR;
  default:
    return -1;
  }
}

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

  const auto readLen = GXGetTexBufferSize(m_width, m_height, RetroFormatToGXFormat(m_format), m_numMips > 0, m_numMips);
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
  const auto writeLen =
      GXGetTexBufferSize(m_width, m_height, RetroFormatToGXFormat(m_format), m_numMips > 0, m_numMips);
  out.writeUBytes(m_data.get(), writeLen);

  return !out.hasError();
}

bool Texture::writeUncooked(const std::string_view path) const {
  if (m_format == ETexelFormat::C4 || m_format == ETexelFormat::C8 || m_format == ETexelFormat::C14X2) {
    return false;
  }

  const auto p = rawPath(path);
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    return false;
  }

  FILE* fp = fopen(p.generic_string().c_str(), "wb");

  png_infop info = png_create_info_struct(png);
  png_init_io(png, fp);

  if (m_format == ETexelFormat::I4 || m_format == ETexelFormat::I8) {
    png_set_IHDR(png, info, m_width, m_height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
  } else {
    png_set_IHDR(png, info, m_width, m_height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  }

  png_write_info(png, info);
  png_set_compression_level(png, 0);

  auto tex =
      convert_texture(RetroFormatToGXFormat(m_format), m_width, m_height, m_numMips,
                      ArrayRef(m_data.get(), GXGetTexBufferSize(m_width, m_height, RetroFormatToGXFormat(m_format),
                                                                m_numMips > 0, m_numMips)));

  std::vector<png_bytep> rowPointers(m_height);
  int stride = m_width;
  if (m_format != ETexelFormat::I4 && m_format != ETexelFormat::I8) {
    stride *= 4;
  }

  for (int y = 0; y < m_height; y++) {
    rowPointers[m_height - 1 - y] = (png_bytep)(tex.data() + y * stride);
  }

  png_write_rows(png, rowPointers.data(), m_height);

  png_write_end(png, nullptr);
  png_write_flush(png);
  png_destroy_write_struct(&png, &info);
  fflush(fp);
  fclose(fp);

  static bool wroteTest = false;
  if (!wroteTest && path.contains("defaultshadow")) {
    FILE* fp = fopen("test.data", "wb");
    fwrite(tex.data(), tex.size(), 1, fp);
    fclose(fp);
    wroteTest = true;
  }

  return true;
}

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