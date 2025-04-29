#pragma once

#include "PrimeDep/IResource.hpp"
namespace axdl::primedep ::MetroidPrime {
class AudioTranslationTable final
: public TypedResource('ATBL', ".rawatbl", ".atbl", DESCRIPTION("Audio Translation Table")) {
public:
  AudioTranslationTable(const char* ptr, std::size_t size);
  AudioTranslationTable(const nlohmann::ordered_json& json);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata);
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  std::vector<uint16_t> m_sfxIds;
};
} // namespace axdl::primedep::MetroidPrime