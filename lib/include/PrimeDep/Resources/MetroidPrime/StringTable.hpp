#pragma once
#include "PrimeDep/IResource.hpp"

namespace axdl::primedep::MetroidPrime {
class StringTable final
: public TypedResource('STRG', ".string_table", ".strg", DESCRIPTION("Translatable String Table")) {
public:
  static constexpr uint32_t kMagicNum = 0x87654321;
  enum class EVersion {
    MetroidPrime1,
    MetroidPrime2,
    MetroidPrime3,
  };

  struct StringPool {
    std::vector<uint32_t> m_stringOffsets;
    std::unique_ptr<char16_t[]> m_strings;
    uint32_t m_bufLen;
  };

  StringTable(const char* ptr, std::size_t size);

  std::u16string string(uint32_t idx, const FourCC& lang = FOURCC('ENGL')) const;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);
  [[nodiscard]] bool writeUncooked(std::string_view path) const override;

  nlohmann::ordered_json metadata(std::string_view repPath) const override;
  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }

private:
  uint32_t m_magic;
  EVersion m_version;
  std::map<FourCC, StringPool> m_languages;
};
} // namespace axdl::primedep::MetroidPrime
