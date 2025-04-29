#include "PrimeDep/Resources/MetroidPrime/MapWorld.hpp"

#include "PrimeDep/ResourcePool.hpp"

#include <athena/FileReader.hpp>
#include <athena/MemoryReader.hpp>

namespace axdl::primedep ::MetroidPrime {
MapWorld::MapWorld(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  const auto magic = in.readUint32Big();
  assert(magic == kMapWorldMagic && "Unexpected magic");
  const auto version = in.readUint32Big();
  assert(version == kMapWorldVersion && "Unexpected version");

  auto mapAreaCount = in.readUint32Big();
  while (mapAreaCount--) {
    m_mapAreas.emplace_back(in, FOURCC('MAPA'));
  }
}

MapWorld::MapWorld(const nlohmann::ordered_json& in) {

  for (const auto areas = in["Areas"]; const auto& area : areas) {
    m_mapAreas.emplace_back(area, FOURCC('MAPA'));
  }
}

bool MapWorld::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());

  out.writeUint32Big(m_mapAreas.size());

  for (const auto& area : m_mapAreas) {
    area.PutTo(out);
  }

  return !out.hasError();
}

bool MapWorld::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json j;
  for (const auto& area : m_mapAreas) {
    area.PutTo(j["Areas"].emplace_back(), FOURCC('MAPA'));
  }
  athena::io::FileWriter writer(p.generic_string());
  const auto js = j.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> MapWorld::loadCooked(const char* data, std::size_t size) {
  return std::make_shared<MapWorld>(data, size);
}

bool MapWorld::canIngest(const nlohmann::ordered_json& metadata) {
  return metadata["ResourceType"] == ResourceType().toString();
}
std::shared_ptr<IResource> MapWorld::ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
  const auto p = GetRawPath(path);
  athena::io::FileReader in(p.generic_string());
  auto js = nlohmann::ordered_json::parse(in.readString());
  if (!js.contains("Areas")) {
    return nullptr;
  }

  return std::make_shared<MapWorld>(js);
}

nlohmann::ordered_json MapWorld::metadata(const std::string_view repPath) const {
  auto out = ITypedResource::metadata(repPath);
  out["Version"] = kMapWorldVersion;
  return out;
}

} // namespace axdl::primedep::MetroidPrime