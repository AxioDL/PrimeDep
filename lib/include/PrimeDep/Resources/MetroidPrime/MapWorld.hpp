#pragma once
#include "PrimeDep/IResource.hpp"

namespace axdl::primedep::MetroidPrime {
class MapWorld final : public TypedResource('MAPW', ".mapworld", ".mapw", DESCRIPTION("AutoMapper Map List")) {
public:
  static constexpr uint32_t kMapWorldMagic = 0xDEADF00D;
  static constexpr uint32_t kMapWorldVersion = 1;

  MapWorld(const char* ptr, std::size_t size);
  explicit MapWorld(const nlohmann::ordered_json& in);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* data, std::size_t size);
  static bool canIngest(const nlohmann::ordered_json& metadata);
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

  nlohmann::ordered_json metadata(std::string_view repPath) const override;

private:
  std::vector<AssetId32Big> m_mapAreas;
};
} // namespace axdl::primedep::MetroidPrime
