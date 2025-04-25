#pragma once
#include "PrimeDep/IResource.hpp"

namespace axdl::primedep {
class MapWorld final : public ITypedResource<FOURCC('MAPW'), "AutoMapper Map List"> {
public:
  static constexpr uint32_t kMapWorldMagic = 0xDEADF00D;
  static constexpr uint32_t kMapWorldVersion = 1;

  MapWorld(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc);

  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* data, std::size_t size, const ResourceDescriptor32Big& desc);
  static bool canInjest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> injest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }

  nlohmann::ordered_json metadata(std::string_view repPath) const override;

private:
  std::vector<AssetId32Big> m_mapAreas;
};
} // namespace axdl::primedep
