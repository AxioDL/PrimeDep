#pragma once

#include "PrimeDep/IResource.hpp"

namespace axdl::primedep ::MetroidPrime {
class RasterFont final : public TypedResource('FONT', ".rpfr", ".rpff", DESCRIPTION("Raster Font")) {
public:
  RasterFont(const char* ptr, std::size_t size);

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size) {
    return std::make_shared<RasterFont>(ptr, size);
  }

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }

  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }
};
} // namespace axdl::primedep::MetroidPrime