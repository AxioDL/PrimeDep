#include "PrimeDep/Resources/MetroidPrime/CharLayoutInfo.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
CharLayoutInfo::CharLayoutInfo(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> CharLayoutInfo::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<CharLayoutInfo>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime