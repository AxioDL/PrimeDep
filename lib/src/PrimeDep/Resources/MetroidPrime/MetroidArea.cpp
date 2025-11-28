#include "PrimeDep/Resources/MetroidPrime/MetroidArea.hpp"

#include "athena/MemoryReader.hpp"

#include <iostream>

namespace axdl::primedep::MetroidPrime {
MetroidArea::MetroidArea(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  const auto magic = in.readUint32Big();

  if (magic != kMagicNumber) {
    return;
  }

  m_version = static_cast<Version>(in.readUint32Big());
  if (m_version != Version::Prime) {
    return;
  }

  m_areaTransform.loadBig(in);
}
std::shared_ptr<IResource> MetroidArea::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<MetroidArea>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime