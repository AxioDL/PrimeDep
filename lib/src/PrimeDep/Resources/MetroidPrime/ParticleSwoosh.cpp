#include "PrimeDep/Resources/MetroidPrime/ParticleSwoosh.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime {
ParticleSwoosh::ParticleSwoosh(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> ParticleSwoosh::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<ParticleSwoosh>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime