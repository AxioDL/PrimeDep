#include "PrimeDep/Resources/MetroidPrime/CollidableOBBTreeGroup.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
CollidableOBBTreeGroup::CollidableOBBTreeGroup(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> CollidableOBBTreeGroup::loadCooked(const char* ptr, std::size_t size,
                                                            const ResourceDescriptor32Big& desc) {
  return std::make_shared<CollidableOBBTreeGroup>(ptr, size, desc);
}
} // namespace axdl::primedep