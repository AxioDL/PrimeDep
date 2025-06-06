#pragma once

#include "PrimeDep/IResource.hpp"
#include "nlohmann/json_fwd.hpp"
#include <athena/IStreamReader.hpp>
#include <libpng16/png.h>

namespace axdl::primedep ::MetroidPrime {
enum class EPaletteFormat {
  IA8 = 0,
  RGB565 = 1,
  RGB5A3 = 2,
};

enum class ETexelFormat {
  Invalid = -1,
  I4 = 0,
  I8 = 1,
  IA4 = 2,
  IA8 = 3,
  C4 = 4,
  C8 = 5,
  C14X2 = 6,
  RGB565 = 7,
  RGB5A3 = 8,
  RGBA8 = 9,
  CMPR = 10,
  // Metaforce addition: non-converting formats
  RGBA8PC = 11,
  R8PC = 12,
};

class GraphicsPalette final {
public:
  explicit GraphicsPalette(athena::io::IStreamReader& in);

  void PutTo(athena::io::IStreamWriter& out) const;
  void PutTo(nlohmann::ordered_json& out) const;
  [[nodiscard]] uint32_t entryCount() const { return m_width * m_height; }
  uint16_t* entries() const { return m_entries.get(); }

  [[nodiscard]] EPaletteFormat format() const { return m_format; }
  [[nodiscard]] uint16_t width() const { return m_width; }
  void setWidth(const uint16_t width) { m_width = width; }
  [[nodiscard]] uint16_t height() const { return m_height; }
  void setHeight(const uint16_t height) { m_height = height; }

private:
  EPaletteFormat m_format;
  uint16_t m_width;
  uint16_t m_height;
  std::unique_ptr<uint16_t[]> m_entries;
};

class Texture final : public TypedResource('TXTR', ".png", ".txtr", DESCRIPTION("Texture Image Resource")) {
public:
  Texture(const char* ptr, std::size_t size);

  [[nodiscard]] ETexelFormat format() const { return m_format; }
  [[nodiscard]] uint16_t width() const { return m_width; }
  [[nodiscard]] uint16_t height() const { return m_height; }

  [[nodiscard]] uint32_t numMips() const { return m_numMips; }

  [[nodiscard]] bool writeCooked(std::string_view path) const override;
  [[nodiscard]] bool writeUncooked(std::string_view path) const override;

  [[nodiscard]] nlohmann::ordered_json metadata(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);
  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest([[maybe_unused]] const nlohmann::ordered_json& metadata,
                                           [[maybe_unused]] std::string_view path) {
    return nullptr;
  }

private:
  [[nodiscard]] bool writeUncookedImageIndexed(std::string_view path) const;
  [[nodiscard]] bool writeUncookedImage(std::string_view path) const;
  ETexelFormat m_format;
  uint16_t m_width;
  uint16_t m_height;
  uint32_t m_numMips;
  std::optional<GraphicsPalette> m_palette;
  std::unique_ptr<uint8_t[]> m_data;
};
}; // namespace axdl::primedep::MetroidPrime