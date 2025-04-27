#include "PrimeDep/Resources/MetroidPrime/PathFindArea.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime {
PathFindArea::PathFindArea(const char* ptr, const std::size_t size) { athena::io::MemoryReader mr(ptr, size, true); }
std::shared_ptr<IResource> PathFindArea::loadCooked(const char* ptr, const std::size_t size) {
  return std::make_shared<PathFindArea>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime