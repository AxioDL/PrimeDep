#include "PrimeDep/Resources/MetroidPrime/SaveWorld.hpp"

#include "../../../../../cmake-build-debug/_deps/tinygltf-src/json.hpp"
#include "PrimeDep/Resources/MetroidPrime/AnimPOIData.hpp"
#include "athena/MemoryReader.hpp"

#include <iostream>
#include <magic_enum/magic_enum.hpp>

namespace axdl::primedep::MetroidPrime {
SaveWorld::LayerToggle::LayerToggle(athena::io::IStreamReader& in)
: internalAreaId(in.readInt32Big()), layerIndex(in.readInt32Big()) {}

SaveWorld::LayerToggle::LayerToggle(const nlohmann::ordered_json& in)
: internalAreaId(in.value("InternalAreaId", -1)), layerIndex(in.value("LayerIndex", -1)) {}

void SaveWorld::LayerToggle::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(internalAreaId);
  out.writeUint32Big(layerIndex);
}

void SaveWorld::LayerToggle::PutTo(nlohmann::ordered_json& out) const {
  out["InternalAreaId"] = internalAreaId;
  out["LayerIndex"] = layerIndex;
}

SaveWorld::ScannableObject::ScannableObject(athena::io::IStreamReader& in)
: scanAssetId(in, ScannableObjectInfo::ResourceType())
, category(static_cast<ScannableObjectInfo::ECategory>(in.readUint32Big())) {}

SaveWorld::ScannableObject::ScannableObject(const nlohmann::ordered_json& in)
: scanAssetId(in.value("Scan", nlohmann::ordered_json::object()), ScannableObjectInfo::ResourceType())
, category(magic_enum::enum_cast<ScannableObjectInfo::ECategory>(in.value("Category", ""), magic_enum::case_insensitive)
               .value_or(ScannableObjectInfo::ECategory::NoLog)) {}

void SaveWorld::ScannableObject::PutTo(athena::io::IStreamWriter& out) const {
  scanAssetId.PutTo(out);
  out.writeUint32Big(static_cast<uint32_t>(category));
}

void SaveWorld::ScannableObject::PutTo(nlohmann::ordered_json& out) const {
  scanAssetId.PutTo(out["Scan"], ScannableObjectInfo::ResourceType());
  out["Category"] = magic_enum::enum_name(category);
}

SaveWorld::SaveWorld(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  const auto magic = in.readUint32Big();
  if (magic != kMagicNumber) {
    return;
  }

  m_version = static_cast<Version>(in.readUint32Big());
  if (m_version != Version::Prime) {
    return;
  }

  m_areaCount = in.readUint32Big();
  auto skippableCount = in.readUint32Big();
  while (skippableCount--) {
    m_skippableCinematics.emplace_back(in.readUint32Big());
  }

  auto relayCount = in.readUint32Big();
  while (relayCount--) {
    m_memoryRelays.emplace_back(in.readUint32Big());
  }

  auto layerCount = in.readUint32Big();
  while (layerCount--) {
    m_layerToggles.emplace_back(in);
  }

  auto doorCount = in.readUint32Big();
  while (doorCount--) {
    m_doors.emplace_back(in.readUint32Big());
  }

  auto scannableCount = in.readUint32Big();
  while (scannableCount--) {
    m_scannableObjects.emplace_back(in);
  }
}

[[nodiscard]] bool SaveWorld::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json out;

  out["Version"] = magic_enum::enum_name(m_version);
  out["AreaCount"] = m_areaCount;
  auto& cinematics = out["SkippableCinematics"];
  for (const auto& cinematic : m_skippableCinematics) {
    cinematics.push_back(cinematic.Id());
  }

  auto& relays = out["MemoryRelays"];
  for (const auto& relay : m_memoryRelays) {
    relays.emplace_back(relay.Id());
  }

  auto& layers = out["LayerToggles"];
  for (const auto& layer : m_layerToggles) {
    layer.PutTo(layers.emplace_back());
  }

  auto& doors = out["Doors"];
  for (const auto& door : m_doors) {
    doors.emplace_back(door.Id());
  }

  auto& scannableObjects = out["ScannableObjects"];
  for (const auto& scannableObject : m_scannableObjects) {
    scannableObject.PutTo(scannableObjects.emplace_back());
  }
  athena::io::FileWriter writer(p.generic_string());
  std::string json = out.dump(4);
  writer.writeString(json, json.length());
  return !writer.hasError();
}
[[nodiscard]] bool SaveWorld::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter writer(p.generic_string());
  writer.writeUint32Big(kMagicNumber);
  writer.writeUint32Big(static_cast<uint32_t>(m_version));
  writer.writeUint32Big(m_areaCount);
  writer.writeUint32Big(m_skippableCinematics.size());

  for (const auto& cinematic : m_skippableCinematics) {
    writer.writeUint32Big(cinematic.id);
  }

  writer.writeUint32Big(m_memoryRelays.size());
  for (const auto& relay : m_memoryRelays) {
    writer.writeUint32Big(relay.id);
  }

  writer.writeUint32Big(m_layerToggles.size());
  for (const auto& layer : m_layerToggles) {
    layer.PutTo(writer);
  }

  writer.writeUint32Big(m_doors.size());
  for (const auto& door : m_doors) {
    writer.writeUint32Big(door.id);
  }

  writer.writeUint32Big(m_scannableObjects.size());
  for (const auto& scannableObject : m_scannableObjects) {
    scannableObject.PutTo(writer);
  }

  return !writer.hasError();
}

std::shared_ptr<IResource> SaveWorld::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<SaveWorld>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime