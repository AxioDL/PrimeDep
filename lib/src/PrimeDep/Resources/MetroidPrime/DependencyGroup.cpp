#include "PrimeDep/Resources/MetroidPrime/DependencyGroup.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
DependencyGroup::DependencyGroup(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> DependencyGroup::loadCooked(const char* ptr, std::size_t size,
                                                            const ResourceDescriptor32Big& desc) {
  return std::make_shared<DependencyGroup>(ptr, size, desc);
}
} // namespace axdl::primedep::MetroidPrime