#include "PrimeDep/Resources/MetroidPrime/CollisionResponseData.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
CollisionResponseData::CollisionResponseData(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> CollisionResponseData::loadCooked(const char* ptr, std::size_t size,
                                                             const ResourceDescriptor32Big& desc) {
  return std::make_shared<CollisionResponseData>(ptr, size, desc);
}
} // namespace axdl::primedep