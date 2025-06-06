#pragma once

#include "PrimeDep/IResource.hpp"

namespace axdl::primedep::MetroidPrime {
class AudioGroup final : public TypedResource('AGSC', ".agsr", ".agsc", DESCRIPTION("MusyX Audio Group")) {
public:
  AudioGroup(const char* data, std::size_t size);
  explicit AudioGroup(const nlohmann::ordered_json& in);

  static std::shared_ptr<IResource> loadCooked(const char* data, std::size_t size);

  [[nodiscard]] bool writeUncooked(std::string_view path) const override;
  [[nodiscard]] bool writeCooked(std::string_view path) const override;

  [[nodiscard]] nlohmann::ordered_json metadata(std::string_view path) const override;

  static bool canIngest(const nlohmann::ordered_json& metadata);
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  std::string m_moduleDir;
  std::string m_moduleName;
  std::unique_ptr<uint8_t[]> m_pool;
  uint32_t m_poolSize;
  std::unique_ptr<uint8_t[]> m_project;
  uint32_t m_projectSize;
  std::unique_ptr<uint8_t[]> m_samples;
  uint32_t m_samplesSize;
  std::unique_ptr<uint8_t[]> m_sampleDir;
  uint32_t m_sampleDirSize;
};
} // namespace axdl::primedep::MetroidPrime
