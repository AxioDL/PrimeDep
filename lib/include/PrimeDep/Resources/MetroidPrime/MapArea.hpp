#pragma once

#include "PrimeDep/Graphics/GXTypes.hpp"
#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Math/AABox.hpp"
#include "PrimeDep/Math/Vector3f.hpp"
#include "PrimeDep/WorldCommon/MetroidPrime/MappableObject.hpp"

namespace axdl::primedep::MetroidPrime {
class MapArea final : public TypedResource('MAPA', ".maparea", ".mapa", DESCRIPTION("Metroid Map Area")) {
public:
  enum class EVisMode {
    Always,
    MapStationOrVisit,
    Visit,
    Never,
  };

  struct MapAreaSurface {
    struct Primitive {
      EPrimitiveType m_type{};
      std::vector<uint8_t> m_indices;
      Primitive() = default;
      explicit Primitive(EPrimitiveType type, const std::vector<uint8_t>& indices) : m_type(type), m_indices(indices) {}
      explicit Primitive(athena::io::IStreamReader& in);
    };

    struct Border {
      std::vector<uint8_t> m_indices;
      Border() = default;
      explicit Border(const std::vector<uint8_t>& indices) : m_indices(indices) {}
      explicit Border(athena::io::IStreamReader& in);
    };

    Vector3f m_normal;
    Vector3f m_centroid;
    uint32_t m_surfaceOffset = 0;
    uint32_t m_lineOffset = 0;
    std::vector<Primitive> m_primitives;
    std::vector<Border> m_borders;

    MapAreaSurface() = default;
    explicit MapAreaSurface(const Vector3f& normal, const Vector3f& centroid, const std::vector<Primitive>& primitives,
                            const std::vector<Border>& borders)
    : m_normal(normal), m_centroid(centroid), m_primitives(primitives), m_borders(borders) {}
    explicit MapAreaSurface(athena::io::IStreamReader& in);

    Vector3f& normal() { return m_normal; }
    const Vector3f& normal() const { return m_normal; }
    void setNormal(const Vector3f& normal) { m_normal = normal; }

    Vector3f& centroid() { return m_centroid; }
    const Vector3f& centroid() const { return m_centroid; }
    void setCentroid(const Vector3f& centroid) { m_centroid = centroid; }

    uint32_t surfaceOffset() const { return m_surfaceOffset; }
    void setSurfaceOffset(const uint32_t offset) { m_surfaceOffset = offset; }

    uint32_t lineOffset() const { return m_lineOffset; }
    void setLineOffset(const uint32_t offset) { m_lineOffset = offset; }

    std::vector<Primitive>& primitives() { return m_primitives; }
    const std::vector<Primitive>& primitives() const { return m_primitives; }

    std::vector<Border>& borders() { return m_borders; }
    const std::vector<Border>& borders() const { return m_borders; }
  };

  static constexpr uint32_t kMagicNumber = 0xDEADD00D;
  MapArea(const char* ptr, std::size_t size);

  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }

  nlohmann::ordered_json metadata(std::string_view repPath) const override {
    auto ret = ITypedResource::metadata(repPath);
    ret["Version"] = m_version;
    return ret;
  }

private:
  uint32_t m_version;
  EVisMode m_visMode{};
  AABox m_bounds;
  std::vector<MappableObject> m_mappableObjects;
  std::vector<Vector3f> m_vertices;
  std::vector<MapAreaSurface> m_surfaces;
};
} // namespace axdl::primedep::MetroidPrime