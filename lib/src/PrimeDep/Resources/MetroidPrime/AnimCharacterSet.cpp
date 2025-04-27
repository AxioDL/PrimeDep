#include "PrimeDep/Resources/MetroidPrime/AnimCharacterSet.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AnimCharacterSet::AnimCharacterSet(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> AnimCharacterSet::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<AnimCharacterSet>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime