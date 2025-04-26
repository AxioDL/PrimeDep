#include "PrimeDep/Resources/MetroidPrime/ProjectileWeapon.hpp"

namespace axdl::primedep ::MetroidPrime{
ProjectileWeapon::ProjectileWeapon(const char* data, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  delete[] data;
}

} // namespace axdl::primedep