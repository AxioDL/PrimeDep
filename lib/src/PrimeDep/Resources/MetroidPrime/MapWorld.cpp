#include "PrimeDep/Resources/MetroidPrime/MapWorld.hpp"

#include "PrimeDep/ResourcePool.hpp"

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
    m_mapAreas.emplace_back(in);
  }
}

bool MapWorld::writeUncooked(std::string_view path) const {
  std::filesystem::path p = rawPath(path);
  nlohmann::ordered_json j;
  for (const auto& area : m_mapAreas) {
    const auto map = ResourcePool32Big::instance()->resourceById(ObjectTag32Big(FOURCC('MAPA'), area));
    j["Areas"].emplace_back(map->rawPath(map->repPath()));
  }
  athena::io::FileWriter writer(p.generic_string());
  const auto js = j.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> MapWorld::loadCooked(const char* data, std::size_t size) {
  return std::make_shared<MapWorld>(data, size);
}

nlohmann::ordered_json MapWorld::metadata(const std::string_view repPath) const {
  auto out = ITypedResource::metadata(repPath);
  out["Version"] = kMapWorldVersion;
  return out;
}

} // namespace axdl::primedep::MetroidPrime