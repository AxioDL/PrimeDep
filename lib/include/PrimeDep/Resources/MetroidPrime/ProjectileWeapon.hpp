#pragma once

#include "PrimeDep/IResource.hpp"

namespace axdl::primedep ::MetroidPrime {
class ProjectileWeapon
: public TypedResource('WPSC', ".wpsm", ".wpsm.wpsc", DESCRIPTION("Projectile Weapon Definition")) {
public:
  ProjectileWeapon(const char* data, std::size_t size);

  static std::shared_ptr<IResource> loadCooked(const char* data, const std::size_t size) {
    return std::make_shared<ProjectileWeapon>(data, size);
  }

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }

  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }
};
} // namespace axdl::primedep::MetroidPrime