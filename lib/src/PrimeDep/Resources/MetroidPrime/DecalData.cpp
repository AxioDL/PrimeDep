#include "PrimeDep/Resources/MetroidPrime/DecalData.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
DecalData::DecalData(const char* ptr, const std::size_t size) { athena::io::MemoryReader mr(ptr, size, true); }
std::shared_ptr<IResource> DecalData::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<DecalData>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime