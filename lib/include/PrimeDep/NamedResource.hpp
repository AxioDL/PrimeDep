#pragma once
#include <cstdint>

namespace axdl::primedep {
template <class AssetIdType, bool BigEndian>
class NamedResource {
public:
  NamedResource(const std::string_view name, const FourCC& type, const AssetIdType& assetId)
  : m_name(name), m_type(type), m_assetId(assetId) {}
  explicit NamedResource(athena::io::IStreamReader& in) {
    m_type = FourCC(in);
    m_assetId = AssetIdType(in, m_type);
    const uint32_t nameLen = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_name = in.readString(nameLen);
  }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string_view name) { m_name = name; }
  [[nodiscard]] const FourCC& type() const { return m_type; }
  void setType(const FourCC& type) { m_type = type; }
  [[nodiscard]] const AssetIdType& assetId() const { return m_assetId; }
  void setAssetId(const AssetIdType& assetId) { m_assetId = assetId; }

private:
  std::string m_name;
  FourCC m_type;
  AssetIdType m_assetId;
};

using NamedResource32Big = NamedResource<AssetId32Big, true>;

} // namespace axdl::primedep