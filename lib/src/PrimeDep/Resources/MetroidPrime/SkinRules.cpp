#include "PrimeDep/Resources/MetroidPrime/SkinRules.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime {
SkinRules::SkinRules(const char* ptr, const std::size_t size) { athena::io::MemoryReader mr(ptr, size, true); }
std::shared_ptr<IResource> SkinRules::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<SkinRules>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime