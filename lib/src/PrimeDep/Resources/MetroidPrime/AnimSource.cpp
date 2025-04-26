#include "PrimeDep/Resources/MetroidPrime/AnimSource.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AnimSource::AnimSource(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc) : ITypedResource(desc) {
  athena::io::MemoryReader(ptr, size, true);
}

std::shared_ptr<IResource> AnimSource::loadCooked(const char* ptr, const std::size_t size,
                                                  const ResourceDescriptor32Big& desc) {

  return std::make_shared<AnimSource>(ptr, size, desc);
}

} // namespace axdl::primedep