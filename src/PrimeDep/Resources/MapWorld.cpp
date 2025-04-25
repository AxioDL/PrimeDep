#include "PrimeDep/Resources/MapWorld.hpp"

#include <athena/MemoryReader.hpp>

namespace axdl::primedep {
MapWorld::MapWorld(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
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
  std::filesystem::path p(path);
  while (p.has_extension()) {
    p.replace_extension();
  }
  p.replace_extension(".mapw");

  nlohmann::ordered_json j;
  for (const auto& area : m_mapAreas) {
    j["Areas"].emplace_back(ResourceNameDatabase::instance().pathForAsset(ObjectTag32Big(FOURCC('MAPA'), area)));
  }
  athena::io::FileWriter writer(p.generic_string());
  std::string js = j.dump(4);
  writer.writeString(js, js.length());
  return true;
}

std::shared_ptr<IResource> MapWorld::loadCooked(const char* data, std::size_t size,
                                                const ResourceDescriptor32Big& desc) {
  return std::make_shared<MapWorld>(data, size, desc);
}

nlohmann::ordered_json MapWorld::metadata(const std::string_view repPath) const {
  auto out = ITypedResource::metadata(repPath);
  out["Version"] = kMapWorldVersion;
  return out;
}

} // namespace axdl::primedep