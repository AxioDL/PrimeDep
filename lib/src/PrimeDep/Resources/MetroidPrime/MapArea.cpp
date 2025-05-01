#include "PrimeDep/Resources/MetroidPrime/MapArea.hpp"

#include "athena/MemoryReader.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "magic_enum/magic_enum.hpp"

#include <glm/glm.hpp>
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
: m_normal(Vector3f(in))
, m_centroid(Vector3f(in))
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
  m_bounds = AABox(in);
  auto mappableObjectCount = in.readUint32Big();
  auto vertexCount = in.readUint32Big();
  const auto surfaceCount = in.readUint32Big();
  const auto pos = in.position();

  while (mappableObjectCount--) {
    m_mappableObjects.emplace_back(in);
  }

  while (vertexCount--) {
    m_vertices.emplace_back(Vector3f(in));
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
glm::mat4 zUpToYUpMatrix() { return glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); }
glm::mat4 yUpToZUpMatrix() { return glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); }

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

  std::vector<Vector3f> vertices;
  glm::mat4 conversionMatrix = zUpToYUpMatrix();

  for (const auto& vertex : m_vertices) {
    glm::vec4 vec{vertex.x(), vertex.y(), vertex.z(), 1.f};
    vec = conversionMatrix * vec;
    vertices.emplace_back(vec.x, vec.y, vec.z);
  }
  tinygltf::Model model;
  auto& vertexBuffer = model.buffers.emplace_back();
  vertexBuffer.data.resize(m_vertices.size() * sizeof(Vector3f));
  memcpy(vertexBuffer.data.data(), vertices.data(), vertices.size() * sizeof(Vector3f));
  auto& vertexBufferView = model.bufferViews.emplace_back();
  vertexBufferView.buffer = 0;
  vertexBufferView.byteOffset = 0;
  vertexBufferView.byteLength = vertices.size() * sizeof(Vector3f);
  vertexBufferView.byteStride = sizeof(Vector3f);
  vertexBufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

  auto& vertexAccessor = model.accessors.emplace_back();
  vertexAccessor.bufferView = 0;
  vertexAccessor.byteOffset = 0;
  vertexAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
  vertexAccessor.type = TINYGLTF_TYPE_VEC3;
  vertexAccessor.count = vertices.size();

  vertexAccessor.minValues = {m_bounds.min().x(), m_bounds.min().y(), m_bounds.min().z()};
  vertexAccessor.maxValues = {m_bounds.max().x(), m_bounds.max().y(), m_bounds.max().z()};

  auto& scene = model.scenes.emplace_back();
  auto& hullMesh = model.meshes.emplace_back();
  hullMesh.name = "Hull";
  auto& hullNode = model.nodes.emplace_back();
  hullNode.mesh = 0;
  hullNode.name = hullMesh.name;

  for (const auto& surface : m_surfaces) {
    for (const auto& primitive : surface.primitives()) {
      assert(primitive.m_type != EPrimitiveType::Quads && primitive.m_type != EPrimitiveType::Points);

      if (primitive.m_type != EPrimitiveType::Triangles && primitive.m_type != EPrimitiveType::TriangleStrip &&
          primitive.m_type != EPrimitiveType::TriangleFan && primitive.m_type != EPrimitiveType::Lines &&
          primitive.m_type != EPrimitiveType::LineStrip) {
        continue;
      }

      auto& prim = hullMesh.primitives.emplace_back();
      auto& indices = model.buffers.emplace_back();

      auto& indicesView = model.bufferViews.emplace_back();
      auto& indicesAccessor = model.accessors.emplace_back();

      indicesView.buffer = model.buffers.size() - 1;
      indicesView.byteOffset = 0;
      indicesView.byteStride = 1;
      indicesView.byteLength = primitive.m_indices.size();
      indicesView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

      indicesAccessor.bufferView = model.bufferViews.size() - 1;
      indicesAccessor.byteOffset = 0;
      indicesAccessor.count = primitive.m_indices.size();
      indicesAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
      indicesAccessor.type = TINYGLTF_TYPE_SCALAR;

      prim.indices = model.accessors.size() - 1;
      prim.attributes["POSITION"] = 0;

      switch (primitive.m_type) {
      case EPrimitiveType::TriangleFan:
      case EPrimitiveType::TriangleStrip:
      case EPrimitiveType::Triangles: {
        if (primitive.m_type == EPrimitiveType::TriangleStrip) {
          prim.mode = TINYGLTF_MODE_TRIANGLE_STRIP;
        } else if (primitive.m_type == EPrimitiveType::TriangleFan) {
          prim.mode = TINYGLTF_MODE_TRIANGLE_FAN;
        } else if (primitive.m_type == EPrimitiveType::Triangles) {
          prim.mode = TINYGLTF_MODE_TRIANGLES;
        } else if (primitive.m_type == EPrimitiveType::LineStrip) {
          prim.mode = TINYGLTF_MODE_LINE_STRIP;
        } else if (primitive.m_type == EPrimitiveType::Lines) {
          prim.mode = TINYGLTF_MODE_LINE;
        }
        for (const auto id : primitive.m_indices) {
          indices.data.push_back(id);
        }
        break;
      default:
        break;
      }
      }
    }
  }

  auto& borderMesh = model.meshes.emplace_back();
  borderMesh.name = "Border";
  auto& borderNode = model.nodes.emplace_back();
  borderNode.mesh = 1;
  borderNode.name = borderMesh.name;

  for (const auto& surface : m_surfaces) {
    for (const auto& border : surface.m_borders) {
      auto& prim = borderMesh.primitives.emplace_back();
      auto& indices = model.buffers.emplace_back();

      auto& indicesView = model.bufferViews.emplace_back();
      auto& indicesAccessor = model.accessors.emplace_back();

      indicesView.buffer = model.buffers.size() - 1;
      indicesView.byteOffset = 0;
      indicesView.byteStride = 1;
      indicesView.byteLength = border.m_indices.size();
      indicesView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

      indicesAccessor.bufferView = model.bufferViews.size() - 1;
      indicesAccessor.byteOffset = 0;
      indicesAccessor.count = border.m_indices.size();
      indicesAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
      indicesAccessor.type = TINYGLTF_TYPE_SCALAR;

      prim.indices = model.accessors.size() - 1;
      prim.attributes["POSITION"] = 0;
      prim.mode = TINYGLTF_MODE_LINE_STRIP;
      for (const auto id : border.m_indices) {
        indices.data.push_back(id);
      }
    }
  }

  scene.nodes.push_back(0);
  scene.nodes.push_back(1);
  scene.name = modelP.filename().replace_extension().generic_string();
  model.asset.generator = "AxioDL PrimeDep MapArea uncooker";
  model.asset.copyright = "AxioDL Team 2025";

  tinygltf::TinyGLTF gltf;
  gltf.WriteGltfSceneToFile(&model, modelP.generic_string(), false, true, true, false);

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