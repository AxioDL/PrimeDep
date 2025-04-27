#include "PrimeDep/Resources/MetroidPrime/CollisionResponseData.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
CollisionResponseData::CollisionResponseData(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> CollisionResponseData::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<CollisionResponseData>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime