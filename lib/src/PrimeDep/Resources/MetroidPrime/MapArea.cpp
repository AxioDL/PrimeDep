#include "PrimeDep/Resources/MetroidPrime/MapArea.hpp"

#include "athena/MemoryReader.hpp"
#include "magic_enum/magic_enum.hpp"

#include <tiny_gltf.h>

namespace axdl::primedep::MetroidPrime {
MapArea::MapAreaSurface::Primitive::Primitive(athena::io::IStreamReader& in)
: m_type(static_cast<EPrimitiveType>(in.readUint32Big())) {
  const auto indexCount = in.readUint32Big();
  m_indices.resize(indexCount);
  in.readUBytesToBuf(m_indices.data(), indexCount);
  in.seekAlign4();
}

MapArea::MapAreaSurface::Border::Border(athena::io::IStreamReader& in) {
  const auto indexCount = in.readUint32Big();
  m_indices.resize(indexCount);
  in.readUBytesToBuf(m_indices.data(), indexCount);
  in.seekAlign4();
}

MapArea::MapAreaSurface::MapAreaSurface(athena::io::IStreamReader& in)
: m_normal(Vector3f::Load<true>(in))
, m_centroid(Vector3f::Load<true>(in))
, m_surfaceOffset(in.readUint32Big())
, m_lineOffset(in.readUint32Big()) {}

MapArea::MapArea(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  if (const auto magic = in.readUint32Big(); magic != kMagicNumber) {
    return;
  }

  m_version = in.readUint32Big();
  in.readUint32Big();
  m_visMode = static_cast<EVisMode>(in.readUint32Big());
  m_bounds = AABox::Load<true>(in);
  auto mappableObjectCount = in.readUint32Big();
  auto vertexCount = in.readUint32Big();
  const auto surfaceCount = in.readUint32Big();
  const auto pos = in.position();

  while (mappableObjectCount--) {
    m_mappableObjects.emplace_back(in);
  }

  while (vertexCount--) {
    m_vertices.emplace_back(Vector3f::Load<true>(in));
  }

  for (int i = 0; i < surfaceCount; i++) {
    MapAreaSurface& surface = m_surfaces.emplace_back(in);
    const auto tmpPos = in.position();
    in.seek(pos + surface.surfaceOffset(), athena::SeekOrigin::Begin);
    uint32_t primCount = in.readUint32Big();
    while (primCount--) {
      surface.primitives().emplace_back(in);
    }
    in.seek(pos + surface.lineOffset(), athena::SeekOrigin::Begin);
    uint32_t borderCount = in.readUint32Big();
    while (borderCount--) {
      surface.borders().emplace_back(in);
    }
    in.seek(tmpPos, athena::SeekOrigin::Begin);
  }
}

bool MapArea::writeUncooked(std::string_view path) const {
  auto p = rawPath(path);
  auto modelP = p;
  modelP.replace_extension(".gltf");

  nlohmann::ordered_json j;
  j["Version"] = m_version;
  j["VisMode"] = magic_enum::enum_name(m_visMode);
  m_bounds.PutTo(j["Bounds"]);

  auto& objects = j["Objects"];
  for (const auto& object : m_mappableObjects) {
    object.PutTo(objects.emplace_back());
  }

  // TODO: Export model data
  athena::io::FileWriter writer(p.generic_string());
  const auto js = j.dump(4) + "\n";
  writer.writeString(js, js.length());

  return !writer.hasError();
}

std::shared_ptr<IResource> MapArea::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<MapArea>(ptr, size);
}

} // namespace axdl::primedep::MetroidPrime