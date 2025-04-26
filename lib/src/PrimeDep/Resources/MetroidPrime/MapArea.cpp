#include "PrimeDep/Resources/MetroidPrime/MetroidArea.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
MetroidArea::MetroidArea(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> MetroidArea::loadCooked(const char* ptr, std::size_t size,
                                                   const ResourceDescriptor32Big& desc) {
  return std::make_shared<MetroidArea>(ptr, size, desc);
}
} // namespace axdl::primedep::MetroidPrime