#pragma once

#include "PrimeDep/IResource.hpp"
namespace axdl::primedep::MetroidPrime {
class DependencyGroup final : public TypedResource('DGRP', ".dgrp", "", DESCRIPTION("Asset Dependency Group")) {
public:
  DependencyGroup(const char* ptr, std::size_t size);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }

private:
  std::vector<ObjectTag32Big> m_dependencies;
};
} // namespace axdl::primedep::MetroidPrime