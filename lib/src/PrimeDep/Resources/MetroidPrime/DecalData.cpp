#include "PrimeDep/Resources/MetroidPrime/DecalData.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime{
DecalData::DecalData(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> DecalData::loadCooked(const char* ptr, std::size_t size,
                                                            const ResourceDescriptor32Big& desc) {
  return std::make_shared<DecalData>(ptr, size, desc);
}
} // namespace axdl::primedep