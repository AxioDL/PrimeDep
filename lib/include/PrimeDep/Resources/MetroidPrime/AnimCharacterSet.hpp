#pragma once

#include "PrimeDep/IResource.hpp"
namespace axdl::primedep::MetroidPrime {
class AnimCharacterSet final
: public TypedResource('ANCS', ".anim_set", ".acs", DESCRIPTION("Animation Character Set")) {
public:
  AnimCharacterSet(const char* ptr, std::size_t size);
  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }

private:
  uint16_t m_version;
  // CharacterSet m_characterSet;
};
} // namespace axdl::primedep::MetroidPrime