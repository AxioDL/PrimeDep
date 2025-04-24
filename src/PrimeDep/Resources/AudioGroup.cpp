#include "PrimeDep/Resources/AudioGroup.hpp"

#include <source_location>

namespace axdl::primedep {
AudioGroup::AudioGroup(const char* data, std::size_t size) {
  printf("UNINMPLEMENTED %s\n", std::source_location::current().function_name());
}

std::shared_ptr<IResource> AudioGroup::create(const char* data, std::size_t size) {
  return std::make_shared<AudioGroup>(data, size);
}
} // namespace axdl::primedep
