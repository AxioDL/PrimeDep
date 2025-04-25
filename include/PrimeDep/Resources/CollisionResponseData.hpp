#pragma once

#include "PrimeDep/IResource.hpp"
namespace axdl::primedep {
class CollisionResponseData final : public ITypedResource<FOURCC('CRSC'), "Collision Response Data"> {
public:
  CollisionResponseData(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc);
  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc);

  static bool canInjest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> injest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }
};
}