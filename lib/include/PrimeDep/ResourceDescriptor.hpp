#pragma once
#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/FourCC.hpp"

namespace axdl::primedep {
template <class AssetIdType, bool BigEndian>
class ResourceDescriptor {
public:
  ResourceDescriptor() = default;
  explicit ResourceDescriptor(athena::io::IStreamReader& in) {
    // isCompressed is an enum internally, but we can just use a bool
    m_isCompressed = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_type = FourCC(in);
    m_assetId = AssetIdType(in);
    m_dataSize = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_dataOffset = BigEndian ? in.readUint32Big() : in.readUint32Little();
  }

  [[nodiscard]] bool isCompressed() const { return m_isCompressed; }
  // TODO: There is specific criteria that actually determines this, when we get cooking implemented  we'll need to
  // refer to that https://wiki.axiodl.com/w/PAK_(Metroid_Prime)#Compression
  void setCompressed(const bool isCompressed) { m_isCompressed = isCompressed; }
  [[nodiscard]] const FourCC& type() const { return m_type; }
  void setType(const FourCC& type) { m_type = type; }
  [[nodiscard]] const AssetIdType& assetId() const { return m_assetId; }
  void setAssetId(const AssetIdType& assetId) { m_assetId = assetId; }

  [[nodiscard]] uint32_t dataSize() const { return m_dataSize; }
  void setDataSize(const uint32_t dataSize) { m_dataSize = dataSize; }
  [[nodiscard]] uint32_t dataOffset() const { return m_dataOffset; }
  void setDataOffset(const uint32_t dataOffset) { m_dataOffset = dataOffset; }

  bool operator<(const ResourceDescriptor& other) const { return m_assetId < other.assetId(); }

  explicit operator bool() const { return m_assetId != AssetIdType() && m_type != kInvalidFourCC; }

  bool operator==(const ResourceDescriptor& other) const {
    return m_isCompressed == other.m_isCompressed && m_assetId == other.assetId() && m_type == other.m_type;
  }

private:
  bool m_isCompressed{false};
  FourCC m_type;
  AssetIdType m_assetId;
  uint32_t m_dataSize{0};
  uint32_t m_dataOffset{0};
};

using ResourceDescriptor32Big = ResourceDescriptor<AssetId32Big, true>;
} // namespace axdl::primedep

template <>
struct std::hash<axdl::primedep::ResourceDescriptor32Big> {
  std::size_t operator()(const axdl::primedep::ResourceDescriptor32Big& key) const {
    return std::hash<string>()(key.type().toString()) << 32 | key.assetId().id;
  }
};