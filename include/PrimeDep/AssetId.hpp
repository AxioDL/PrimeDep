#pragma once
#include <cstdint>
#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>

namespace axdl::primedep {
template <typename T = uint32_t, bool BigEndian = true>
struct AssetId {
  explicit AssetId(const T id = T(-1)) : id(id) {}
  explicit AssetId(athena::io::IStreamReader& in) {
    if constexpr (std::is_same<T, uint32_t>()) {
      id = BigEndian ? in.readUint32Big() : in.readUint32Little();
    } else if constexpr (std::is_same<T, uint64_t>()) {
      id = BigEndian ? in.readUint64Big() : in.readUint64Little();
    }
  }

  explicit AssetId(athena::io::IStreamWriter& out) {
    if constexpr (std::is_same<T, uint32_t>()) {
      if constexpr (BigEndian) {
        out.writeUint32Big(id);
      } else {
        out.writeUint32Little(id);
      }
    } else if constexpr (std::is_same<T, uint64_t>()) {
      if constexpr (BigEndian) {
        out.writeUint64Big(id);
      } else {
        out.writeUint64Little(id);
      }
    }
  }

  T id{T(-1)};
  bool operator==(const AssetId& other) const { return id == other.id; };
  bool operator<(const AssetId& other) const { return id < other.id; }
  explicit operator bool() const { return id != T(-1); }

  operator T() const { return id; }
};

using AssetId32Big = AssetId<>;
static const AssetId32Big kInvalidAssetId32Big;

} // namespace axdl::primedep