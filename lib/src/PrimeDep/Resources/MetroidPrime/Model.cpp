#include "PrimeDep/Resources/MetroidPrime/Model.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep ::MetroidPrime {
Model::Model(const char* ptr, const std::size_t size) { athena::io::MemoryReader mr(ptr, size, true); }

std::shared_ptr<IResource> Model::loadCooked(const char* ptr, const std::size_t size) {
  return std::make_shared<Model>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime