#pragma once

#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Math/Color.hpp"
#include "PrimeDep/Math/Transform4f.hpp"
namespace axdl::primedep::MetroidPrime {
class MapUniverse final : public TypedResource('MAPU', ".mapuniverse", ".mapu", DESCRIPTION("Metroid Map Universe")) {
public:
  static constexpr uint32_t kMagic = 0xABCDEF01;
  static constexpr uint32_t kVersion = 1;

  struct MapWorldData {
    MapWorldData(athena::io::IStreamReader& in, uint32_t version);
    MapWorldData(const nlohmann::ordered_json& in);
    void PutTo(athena::io::IStreamWriter& out, uint32_t version) const;
    void PutTo(nlohmann::ordered_json& out, uint32_t version) const;

    std::string m_name;
    AssetId32Big m_worldId;
    Transform4f m_transform;
    std::vector<Transform4f> m_areaTransforms;
    Color m_color;
  };
  MapUniverse(const char* ptr, std::size_t size);
  MapUniverse(const nlohmann::ordered_json& in);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata);
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  uint32_t m_version;
  AssetId32Big m_hexId;
  std::vector<MapWorldData> m_worlds;
};
} // namespace axdl::primedep::MetroidPrime