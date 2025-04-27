#include "PrimeDep/Resources/MetroidPrime/Particle.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime {
Particle::Particle(const char* ptr, const std::size_t size) { athena::io::MemoryReader in(ptr, size, true); }
std::shared_ptr<IResource> Particle::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<Particle>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime