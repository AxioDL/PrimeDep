#pragma once

#include "PrimeDep/Animation/BoolPOINode.hpp"
#include "PrimeDep/Animation/Int32POINode.hpp"
#include "PrimeDep/Animation/ParticlePOINode.hpp"
#include "PrimeDep/Animation/SoundPOINode.hpp"
#include "PrimeDep/IResource.hpp"
namespace axdl::primedep::MetroidPrime {
class AnimPOIData final : public TypedResource('EVNT', ".event", ".evnt", DESCRIPTION("Animation POI Data")) {
  static constexpr auto kVersion = 2;

public:
  AnimPOIData(const char* ptr, std::size_t size);
  explicit AnimPOIData(const nlohmann::ordered_json& in);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

  nlohmann::ordered_json metadata(const std::string_view repPath) const override {
    auto ret = ITypedResource::metadata(repPath);
    ret["Version"] = m_version;
    return ret;
  }

private:
  uint32_t m_version{kVersion};
  std::vector<BoolPOINode> m_boolNodes;
  std::vector<Int32POINode> m_int32Nodes;
  std::vector<ParticlePOINode> m_particleNodes;
  std::vector<SoundPOINode> m_soundNodes;
};
} // namespace axdl::primedep::MetroidPrime