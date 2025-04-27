#include "PrimeDep/Resources/MetroidPrime/CollidableOBBTreeGroup.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
CollidableOBBTreeGroup::CollidableOBBTreeGroup(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> CollidableOBBTreeGroup::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<CollidableOBBTreeGroup>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime