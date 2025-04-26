#include "PrimeDep/Resources/MetroidPrime/Particle.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime{
Particle::Particle(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc) : ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> Particle::loadCooked(const char* ptr, std::size_t size,
                                                const ResourceDescriptor32Big& desc) {
  return std::make_shared<Particle>(ptr, size, desc);
}
} // namespace axdl::primedep