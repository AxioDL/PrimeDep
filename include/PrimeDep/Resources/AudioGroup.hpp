#pragma once

#include "PrimeDep/IResource.hpp"

namespace axdl::primedep {
class AudioGroup final : public ITypedResource<FourCC(SBIG('AGSC'))> {
public:
  AudioGroup(const char* data, std::size_t size);

  static std::shared_ptr<IResource> create(const char* data, std::size_t size);

  [[nodiscard]] nlohmann::ordered_json metadata() const override { return {}; }
};
} // namespace axdl::primedep
