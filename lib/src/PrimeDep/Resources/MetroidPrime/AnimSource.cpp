#include "PrimeDep/Resources/MetroidPrime/AnimSource.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AnimSource::AnimSource(const char* ptr, const std::size_t size) { athena::io::MemoryReader(ptr, size, true); }

std::shared_ptr<IResource> AnimSource::loadCooked(const char* ptr, const std::size_t size) {
  return std::make_shared<AnimSource>(ptr, size);
}

} // namespace axdl::primedep::MetroidPrime