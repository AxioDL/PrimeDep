#pragma once
#include <cstdint>

namespace axdl::primedep {
template <typename T, bool BigEndian>
class NamedResource {
public:
  NamedResource(const std::string_view name, const uint32_t type, const AssetId<T, BigEndian> assetId)
  : m_name(name), m_type(type), m_assetId(assetId) {}
  explicit NamedResource(athena::io::IStreamReader& in) {
    m_type = FourCC(in);
    m_assetId = AssetId<T, BigEndian>(in);
    const uint32_t nameLen = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_name = in.readString(nameLen);
  }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string_view name) { m_name = name; }
  [[nodiscard]] const FourCC& type() const { return m_type; }
  void setType(const FourCC& type) { m_type = type; }
  [[nodiscard]] AssetId<T, BigEndian> assetId() const { return m_assetId; }
  void setAssetId(AssetId<T, BigEndian> assetId) { m_assetId = assetId; }

private:
  std::string m_name;
  FourCC m_type;
  AssetId<T, BigEndian> m_assetId;
};

using NamedResource32Big = NamedResource<uint32_t, true>;
using NamedResource32Little = NamedResource<uint32_t, false>;
using NamedResource64Big = NamedResource<uint64_t, true>;
using NamedResource64Little = NamedResource<uint64_t, false>;

} // namespace axdl::primedep