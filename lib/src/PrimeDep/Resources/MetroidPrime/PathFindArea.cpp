#include "PrimeDep/Resources/MetroidPrime/PathFindArea.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime{
PathFindArea::PathFindArea(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> PathFindArea::loadCooked(const char* ptr, std::size_t size,
                                                            const ResourceDescriptor32Big& desc) {
  return std::make_shared<PathFindArea>(ptr, size, desc);
}
} // namespace axdl::primedep