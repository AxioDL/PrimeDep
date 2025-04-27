#include "PrimeDep/Resources/MetroidPrime/ParticleElectric.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime {
ParticleElectric::ParticleElectric(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> ParticleElectric::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<ParticleElectric>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime