#pragma once

#include "PrimeDep/IResource.hpp"

namespace axdl::primedep::MetroidPrime {
class Hints final
: public TypedResource('HINT', ".hintr", ".hints", DESCRIPTION("Hint definitions for in-game events")) {
public:
  static constexpr uint32_t kMagic = 0x00BADBAD;
  static constexpr uint32_t kVersion = 1;
  struct HintLocation {
    explicit HintLocation(athena::io::IStreamReader& in);
    explicit HintLocation(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;

    AssetId32Big m_worldId;
    AssetId32Big m_areaId;
    uint32_t m_roomId;
    AssetId32Big m_stringId;
  };

  struct Hint {
    explicit Hint(athena::io::IStreamReader& in);
    explicit Hint(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;

    std::string m_name;
    float m_immediateTime;
    float m_normalTime;
    AssetId32Big m_stringId;
    uint32_t m_textTime;
    std::vector<HintLocation> m_locations;
  };
  Hints(const char* ptr, std::size_t size);
  explicit Hints(const nlohmann::ordered_json& in);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  std::vector<Hint> m_hints;
};
} // namespace axdl::primedep::MetroidPrime