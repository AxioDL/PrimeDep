#include "PrimeDep/Resources/MetroidPrime/SkinRules.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime{
SkinRules::SkinRules(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc) : ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> SkinRules::loadCooked(const char* ptr, std::size_t size,
                                                 const ResourceDescriptor32Big& desc) {
  return std::make_shared<SkinRules>(ptr, size, desc);
}
} // namespace axdl::primedep