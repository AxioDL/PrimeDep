#include "PrimeDep/Resources/MetroidPrime/MapUniverse.hpp"

#include "athena/FileReader.hpp"
#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
MapUniverse::MapWorldData::MapWorldData(athena::io::IStreamReader& in, const uint32_t version)
: m_name(in.readString()), m_worldId(in, FOURCC('MLVL')), m_transform(in) {
  uint32_t areaCount = in.readUint32Big();
  while (areaCount--) {
    m_areaTransforms.emplace_back(in);
  }

  if (version != 0) {
    m_color = Color(in);
  } else {
    // m_color.fromRGBA32(255 | (uint32_t(m_worldId.id) & 0xFFFFFF00))
  }
}
MapUniverse::MapWorldData::MapWorldData(const nlohmann::ordered_json& in)
: m_name(in.value("Name", ""))
, m_worldId(AssetId32Big(in.value("WorldID", nlohmann::ordered_json()), FOURCC('MLVL')))
, m_transform(in.value("Transform", nlohmann::ordered_json()))
, m_color(in.value("Color", nlohmann::ordered_json())) {
  const auto& areas = in["Areas"];
  for (const auto& area : areas) {
    m_areaTransforms.emplace_back(area);
  }
}

void MapUniverse::MapWorldData::PutTo(athena::io::IStreamWriter& out, uint32_t version) const {
  out.writeString(m_name);
  m_worldId.PutTo(out);
  m_transform.PutTo(out);
  out.writeUint32Big(m_areaTransforms.size());
  for (const auto& area : m_areaTransforms) {
    area.PutTo(out);
  }
  if (version != 0) {
    m_color.PutTo(out);
  }
}

void MapUniverse::MapWorldData::PutTo(nlohmann::ordered_json& out, uint32_t version) const {
  out["Name"] = m_name;
  m_worldId.PutTo(out["WorldID"], FOURCC('MLVL'));
  m_transform.PutTo(out["Transform"]);

  auto& areas = out["Areas"];
  for (const auto& area : m_areaTransforms) {
    area.PutTo(areas.emplace_back());
  }
  if (version != 0) {
    m_color.PutTo(out["Color"]);
  }
}

MapUniverse::MapUniverse(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  uint32_t magic = in.readUint32Big();
  assert(magic == kMagic);
  m_version = in.readUint32Big();
  m_hexId = AssetId32Big(in, FOURCC('MAPA'));
  uint32_t worldCount = in.readUint32Big();
  while (worldCount--) {
    m_worlds.emplace_back(in, m_version);
  }
}

MapUniverse::MapUniverse(const nlohmann::ordered_json& in)
: m_version(in.value("Version", kVersion)), m_hexId(in.value("HexID", nlohmann::ordered_json()), FOURCC('MAPA')) {

  for (const auto& worldData = in.value("WorldData", nlohmann::ordered_json()); const auto& wd : worldData) {
    m_worlds.emplace_back(wd);
  }
}

bool MapUniverse::writeCooked(std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());
  out.writeUint32Big(kMagic);
  out.writeUint32Big(m_version);
  m_hexId.PutTo(out);
  out.writeUint32Big(m_worlds.size());
  for (const auto& w : m_worlds) {
    w.PutTo(out, m_version);
  }

  return !out.hasError();
}

bool MapUniverse::writeUncooked(std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json out;
  out["Version"] = m_version;
  m_hexId.PutTo(out["HexID"], FOURCC('MAPA'));
  auto& worldData = out["WorldData"];
  for (const auto& wd : m_worlds) {
    wd.PutTo(worldData.emplace_back(), m_version);
  }
  athena::io::FileWriter writer(p.generic_string());
  const auto js = out.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> MapUniverse::loadCooked(const char* ptr, const std::size_t size) {
  return std::make_shared<MapUniverse>(ptr, size);
}

bool MapUniverse::canIngest(const nlohmann::ordered_json& metadata) {
  return metadata["ResourceType"] == ResourceType().toString();
}
std::shared_ptr<IResource> MapUniverse::ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
  athena::io::FileReader in(GetRawPath(path).generic_string());
  auto js = nlohmann::ordered_json::parse(in.readString());
  if (!js.contains("Version")) {
    return nullptr;
  }

  return std::make_shared<MapUniverse>(js);
}

} // namespace axdl::primedep::MetroidPrime