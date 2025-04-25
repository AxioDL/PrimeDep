#pragma once

#include "PrimeDep/IResource.hpp"

namespace axdl::primedep {
class AudioGroup final : public ITypedResource<FOURCC('AGSC')> {
public:
  AudioGroup(const char* data, std::size_t size, const ResourceDescriptor32Big& desc);

  static std::shared_ptr<IResource> loadCooked(const char* data, std::size_t size, const ResourceDescriptor32Big& desc);

  [[nodiscard]] bool writeUncooked(std::string_view path) const override;
  [[nodiscard]] bool writeCooked(std::string_view path) const override;

  [[nodiscard]] nlohmann::ordered_json metadata(std::string_view path) const override;

  static bool canInjest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> injest(const nlohmann::ordered_json& metadata, std::string_view repPath) { return nullptr; }

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
} // namespace axdl::primedep
