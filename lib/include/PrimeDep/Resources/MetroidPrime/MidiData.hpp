#pragma once

#include "PrimeDep/IResource.hpp"
namespace axdl::primedep::MetroidPrime {
class MidiData final : public TypedResource('CSNG', ".song", ".csng", DESCRIPTION("MusyX Song Data")) {
public:
  MidiData(const char* ptr, std::size_t size);
  MidiData(const nlohmann::ordered_json& in, std::string_view path);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata);
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

  nlohmann::ordered_json metadata(std::string_view repPath) const override;

  std::optional<std::vector<ObjectTag32Big>> allChildTags() const override { return {{m_soundGroup}}; }

private:
  uint32_t m_setupId{};
  uint32_t m_groupId{};
  ObjectTag32Big m_soundGroup;
  uint32_t m_dataLength{};
  std::unique_ptr<uint8_t[]> m_data;
};
} // namespace axdl::primedep::MetroidPrime