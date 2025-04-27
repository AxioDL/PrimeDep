#include "PrimeDep/Resources/MetroidPrime/ProjectileWeapon.hpp"

namespace axdl::primedep ::MetroidPrime {
ProjectileWeapon::ProjectileWeapon(const char* data, const std::size_t size) { delete[] data; }

} // namespace axdl::primedep::MetroidPrime