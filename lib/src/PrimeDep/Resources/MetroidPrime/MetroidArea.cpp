#include "PrimeDep/Resources/MetroidPrime/MetroidArea.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
MetroidArea::MetroidArea(const char* ptr, const std::size_t size) { athena::io::MemoryReader mr(ptr, size, true); }
std::shared_ptr<IResource> MetroidArea::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<MetroidArea>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime