#pragma once
#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Math/AABox.hpp"
#include "PrimeDep/Math/Transform4f.hpp"
#include "PrimeDep/ObjectTag.hpp"
#include "PrimeDep/ResourceDescriptor.hpp"
#include "SoundGroupData.hpp"

namespace axdl::primedep ::MetroidPrime {
class StringTable;
class MetroidWorld final : public TypedResource('MLVL', ".world", ".mwld", DESCRIPTION("Metroid World Definition")) {
public:
  static constexpr uint32_t kWorldMagic = 0xDEAFBABE;
  enum class EVersion {
    Invalid,
    MetroidPrime1 = 17,
    // MetroidPrime2 = 23,
    // MetroidPrime3 = 25,
  };

  struct Relay {
    uint32_t relayId;
    uint32_t targetId;
    uint16_t message;
    bool active;

    explicit Relay(athena::io::IStreamReader& in);
  };

  struct Dock {
    struct DockReference {
      uint32_t areaIdx;
      uint32_t dockIdx;
      explicit DockReference(athena::io::IStreamReader& in);
    };
    std::vector<DockReference> dockReferences;
    std::array<Vector3f, 4> planeVertices;

    explicit Dock(athena::io::IStreamReader& in);
  };

  struct Area {
    AssetId32Big areaNameId;
    Transform4f transform;
    AABox bounds;
    AssetId32Big areaId;
    uint32_t saveId;
    std::vector<uint16_t> attachedAreas;
    std::vector<ObjectTag32Big> lazyLoadDependencies;
    std::vector<ObjectTag32Big> dependencies;
    std::vector<uint32_t> layerStartIndices;
    std::vector<Dock> docks;

    explicit Area(athena::io::IStreamReader& in);
  };

  struct AreaLayer {
    uint32_t layerCount;
    uint64_t layerBits;
    explicit AreaLayer(athena::io::IStreamReader& in);
  };

  MetroidWorld(const char* ptr, std::size_t size);

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  std::optional<std::vector<ObjectTag32Big>> childTags() const override {
    std::vector<ObjectTag32Big> childTags;
    /* TODO: Acquire child tags, don't just rely on stored values */
    for (const auto& area : m_areas) {
      for (const auto& dep : area.lazyLoadDependencies) {
        childTags.push_back(dep);
      }
      for (const auto& dep : area.dependencies) {
        if (!dep) {
          continue;
        }

        childTags.push_back(dep);
      }
      if (area.areaNameId) {
        childTags.push_back(ObjectTag32Big(FourCC("STRG"sv), area.areaNameId));
      }
      if (area.areaId) {
        childTags.push_back(ObjectTag32Big(FourCC("MREA"sv), area.areaId));
      }
    }
    if (m_skyboxId) {
      childTags.push_back(ObjectTag32Big(FourCC("CMDL"sv), m_skyboxId));
    }
    if (m_worldNameId) {
      childTags.push_back(ObjectTag32Big(FourCC("STRG"sv), m_worldNameId));
    }
    if (m_saveWorldId) {
      childTags.push_back(ObjectTag32Big(FourCC("SAVW"sv), m_saveWorldId));
    }
    return childTags;
  }

  std::optional<std::vector<std::shared_ptr<IResource>>> children() const override;

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }

private:
  uint32_t m_magic{0};
  EVersion m_version{};
  AssetId32Big m_worldNameId;
  std::shared_ptr<StringTable> m_worldName;
  AssetId32Big m_saveWorldId;
  std::shared_ptr<IResource> m_saveWorld;
  AssetId32Big m_skyboxId;
  std::shared_ptr<IResource> m_skybox;
  std::vector<Relay> m_relays;
  std::vector<Area> m_areas;
  AssetId32Big m_mapWorldId;
  std::shared_ptr<IResource> m_mapWorld;
  // TODO: ScriptLayer
  std::vector<SoundGroupData> m_soundGroups;
  std::string m_defaultAudioTrack;
  std::vector<AreaLayer> m_areaLayerFlags;
  std::vector<std::string> m_layerNames;
  std::vector<uint32_t> m_areaLayerStartIndices;
};
} // namespace axdl::primedep::MetroidPrime
