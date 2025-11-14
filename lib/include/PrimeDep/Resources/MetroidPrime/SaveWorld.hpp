#pragma once

#include "PrimeDep/IResource.hpp"

#include "PrimeDep/Resources/MetroidPrime/ScannableObjectInfo.hpp"
#include "PrimeDep/WorldCommon/MetroidPrime/EditorId.hpp"

namespace axdl::primedep::MetroidPrime {
class SaveWorld final : public TypedResource('SAVW', ".saveworld", ".savw", DESCRIPTION("Metroid World Save Data")) {
public:
  enum class Version {
    Prime = 3,
    Echoes = 5,
    Corruption = 6,
  };

  struct LayerToggle {
    int32_t internalAreaId;
    int32_t layerIndex;

    explicit LayerToggle(athena::io::IStreamReader& in);
    explicit LayerToggle(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;
  };

  struct ScannableObject {
    AssetId32Big scanAssetId;
    ScannableObjectInfo::ECategory category;

    explicit ScannableObject(athena::io::IStreamReader& in);
    explicit ScannableObject(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;
  };

  static constexpr uint32_t kMagicNumber = 0xC001D00D;

  SaveWorld(const char* ptr, std::size_t size);
  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  [[nodiscard]] bool writeUncooked(std::string_view path) const;
  [[nodiscard]] bool writeCooked(std::string_view path) const;

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }

private:
  Version m_version;
  uint32_t m_areaCount;
  std::vector<EditorId> m_skippableCinematics;
  std::vector<EditorId> m_memoryRelays;
  std::vector<LayerToggle> m_layerToggles;
  std::vector<EditorId> m_doors;
  std::vector<ScannableObject> m_scannableObjects;
};
} // namespace axdl::primedep::MetroidPrime