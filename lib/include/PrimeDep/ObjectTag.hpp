#pragma once
#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/FourCC.hpp"
#include "athena/FileReader.hpp"
#include "nlohmann/json.hpp"

namespace axdl::primedep {
struct IObjectTag {
  virtual ~IObjectTag() = default;
  virtual void PutTo(nlohmann::ordered_json& j) const = 0;
};

template <typename T, bool BigEndian>
struct ObjectTag : public IObjectTag {
  FourCC type;
  AssetId<T, BigEndian> id;

  ObjectTag() = default;
  ObjectTag(const FourCC& type, const AssetId<T, BigEndian>& id) : type(type), id(id) {}

  bool operator<(const ObjectTag& other) const { return id < other.id; }
  explicit operator bool() const { return type != kInvalidFourCC && id != AssetId<T, BigEndian>(); }
};

struct ObjectTag32Big final : ObjectTag<uint32_t, true> {
  ObjectTag32Big() = default;
  void PutTo(nlohmann::ordered_json& j) const override;
  ObjectTag32Big(const FourCC& type, const AssetId32Big& id) : ObjectTag(type, id) {}

  template <bool Reverse = false>
  static ObjectTag32Big Load(athena::io::IStreamReader& in) {
    ObjectTag32Big ret;
    if constexpr (!Reverse) {
      ret.type = FourCC(in);
      ret.id = AssetId32Big(in);
    } else {
      ret.id = AssetId32Big(in);
      ret.type = FourCC(in);
    }

    return ret;
  }
};

} // namespace axdl::primedep
