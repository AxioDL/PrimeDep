#include "PrimeDep/Resources/MetroidPrime/ParticleElectric.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime{
ParticleElectric::ParticleElectric(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc) : ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> ParticleElectric::loadCooked(const char* ptr, std::size_t size,
                                                const ResourceDescriptor32Big& desc) {
  return std::make_shared<ParticleElectric>(ptr, size, desc);
}
} // namespace axdl::primedep