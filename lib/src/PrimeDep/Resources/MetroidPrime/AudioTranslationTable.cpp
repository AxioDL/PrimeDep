#include "PrimeDep/Resources/MetroidPrime/AudioTranslationTable.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AudioTranslationTable::AudioTranslationTable(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> AudioTranslationTable::loadCooked(const char* ptr, std::size_t size,
                                                            const ResourceDescriptor32Big& desc) {
  return std::make_shared<AudioTranslationTable>(ptr, size, desc);
}
} // namespace axdl::primedep