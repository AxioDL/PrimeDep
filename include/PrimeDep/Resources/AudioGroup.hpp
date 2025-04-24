#pragma once

#include "PrimeDep/Resources/IResource.hpp"

namespace axdl::primedep {
class AudioGroup final : public ITypedResource<'AGSC'> {
public:
  AudioGroup(const char* data, std::size_t size);

  static std::shared_ptr<IResource> create(const char* data, std::size_t size);
};
} // namespace axdl::primedep
