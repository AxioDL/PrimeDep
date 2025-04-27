#include "PrimeDep/Resources/MetroidPrime/MidiData.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
MidiData::MidiData(const char* ptr, const std::size_t size) { athena::io::MemoryReader in(ptr, size, true); }
std::shared_ptr<IResource> MidiData::loadCooked(const char* ptr, const std::size_t size) {
  return std::make_shared<MidiData>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime