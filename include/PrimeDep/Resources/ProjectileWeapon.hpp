#pragma once

#include "PrimeDep/IResource.hpp"

namespace axdl::primedep {
class ProjectileWeapon
: public TypedResource('WPSC', ".wpsm", ".wpsm.wpsc", DESCRIPTION("Projectile Weapon Definition")) {
public:
  ProjectileWeapon(const char* data, std::size_t size, const ResourceDescriptor32Big& desc);

  static std::shared_ptr<IResource> loadCooked(const char* data, std::size_t size, const ResourceDescriptor32Big& desc);
  static bool canInjest()
};
} // namespace axdl::primedep