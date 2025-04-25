#pragma once
#include "athena/FileReader.hpp"
#include "nlohmann/json.hpp"
#include "PrimeDep/FourCC.hpp"
#include "PrimeDep/AssetId.hpp"

namespace axdl::primedep {
template <typename T, bool BigEndian>
struct ObjectTag {
  FourCC type;
  AssetId<T, BigEndian> id;

  ObjectTag() = default;
  ObjectTag(const FourCC& type, const AssetId<T, BigEndian>& id) : type(type), id(id) {}
  template <bool Reverse = false>
  static ObjectTag Load(athena::io::IStreamReader& in) {
    ObjectTag ret;
    if constexpr (!Reverse) {
      ret.type = FourCC(in);
      ret.id = AssetId<T, BigEndian>(in);
    } else {
      ret.id = AssetId<T, BigEndian>(in);
      ret.type = FourCC(in);
    }

    return ret;
  }

  bool operator<(const ObjectTag& other) const { return id < other.id; }
  explicit operator bool() const { return type != kInvalidFourCC && id != AssetId<T, BigEndian>(); }
};

using ObjectTag32Big = ObjectTag<uint32_t, true>;
using ObjectTag32Little = ObjectTag<uint32_t, false>;
using ObjectTag64Big = ObjectTag<uint64_t, true>;
using ObjectTag64Little = ObjectTag<uint64_t, false>;
} // namespace axdl::primedep
