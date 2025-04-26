#include "PrimeDep/Resources/MetroidPrime/MidiData.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
MidiData::MidiData(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> MidiData::loadCooked(const char* ptr, std::size_t size,
                                                            const ResourceDescriptor32Big& desc) {
  return std::make_shared<MidiData>(ptr, size, desc);
}
} // namespace axdl::primedep::MetroidPrime