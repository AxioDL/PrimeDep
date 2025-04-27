#include "PrimeDep/Resources/MetroidPrime/SaveWorld.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
SaveWorld::SaveWorld(const char* ptr, const std::size_t size) { athena::io::MemoryReader mr(ptr, size, true); }
std::shared_ptr<IResource> SaveWorld::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<SaveWorld>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime