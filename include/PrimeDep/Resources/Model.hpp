#pragma once

#include "PrimeDep/IResource.hpp"
namespace axdl::primedep {
class Model final : public ITypedResource<FOURCC('CMDL'), "Render Model Data"> {

public:
  Model(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc);
  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc);

  static bool canInjest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> injest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }
};
} // namespace axdl::primedep