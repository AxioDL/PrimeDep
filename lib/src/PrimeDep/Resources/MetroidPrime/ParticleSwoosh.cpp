#include "PrimeDep/Resources/MetroidPrime/ParticleSwoosh.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime{
ParticleSwoosh::ParticleSwoosh(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> ParticleSwoosh::loadCooked(const char* ptr, std::size_t size,
                                                      const ResourceDescriptor32Big& desc) {
  return std::make_shared<ParticleSwoosh>(ptr, size, desc);
}
} // namespace axdl::primedep