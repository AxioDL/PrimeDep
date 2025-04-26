#pragma once

#include "PrimeDep/Animation/BoolPOINode.hpp"
#include "PrimeDep/Animation/Int32POINode.hpp"
#include "PrimeDep/Animation/ParticlePOINode.hpp"
#include "PrimeDep/Animation/SoundPOINode.hpp"
#include "PrimeDep/IResource.hpp"
namespace axdl::primedep::MetroidPrime {
class AnimPOIData final : public TypedResource('EVNT', ".evnt.json", ".evnt", DESCRIPTION("Animation POI Data")) {
public:
  AnimPOIData(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc);

  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }

  nlohmann::ordered_json metadata(const std::string_view repPath) const override {
    auto ret = ITypedResource::metadata(repPath);
    ret["Version"] = m_version;
    return ret;
  }

private:
  uint32_t m_version;
  std::vector<BoolPOINode> m_boolNodes;
  std::vector<Int32POINode> m_int32Nodes;
  std::vector<ParticlePOINode> m_particleNodes;
  std::vector<SoundPOINode> m_soundNodes;
};
} // namespace axdl::primedep::MetroidPrime