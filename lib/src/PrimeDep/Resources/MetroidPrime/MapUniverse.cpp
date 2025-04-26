#include "PrimeDep/Resources/MetroidPrime/MapUniverse.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
MapUniverse::MapUniverse(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> MapUniverse::loadCooked(const char* ptr, std::size_t size,
                                                            const ResourceDescriptor32Big& desc) {
  return std::make_shared<MapUniverse>(ptr, size, desc);
}
} // namespace axdl::primedep::MetroidPrime