#pragma once

#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Math/Transform4f.hpp"

namespace axdl::primedep::MetroidPrime {
class MetroidArea final : public TypedResource('MREA', ".area", ".mrea", DESCRIPTION("Metroid Area")) {
  static constexpr auto kMagicNumber = 0xDEADBEEF;

public:
  enum class Version {
    Prime = 15,
    Echoes = 19,
    Corruption = 30,
  };

  MetroidArea(const char* ptr, std::size_t size);
  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }

private:
  Version m_version;
  Transform4f m_areaTransform;
};
} // namespace axdl::primedep::MetroidPrime