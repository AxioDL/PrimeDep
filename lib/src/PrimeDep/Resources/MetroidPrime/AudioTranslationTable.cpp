#include "PrimeDep/Resources/MetroidPrime/AudioTranslationTable.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AudioTranslationTable::AudioTranslationTable(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> AudioTranslationTable::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<AudioTranslationTable>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime