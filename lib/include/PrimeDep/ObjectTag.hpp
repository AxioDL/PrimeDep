#pragma once
#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/FourCC.hpp"
#include "athena/IStreamWriter.hpp"
#include "nlohmann/json.hpp"

namespace axdl::primedep {
struct IObjectTag {
  virtual ~IObjectTag() = default;
  virtual void PutTo(athena::io::IStreamWriter& out, bool reversed = false) const = 0;
  virtual void PutTo(nlohmann::ordered_json& j) const = 0;
  virtual std::string_view repPath() const = 0;
};

template <typename T>
struct ObjectTag : public IObjectTag {
  FourCC type;

  ObjectTag() = default;
  ObjectTag(const FourCC& type) : type(type) {}
  ~ObjectTag() = default;

  virtual T id() const = 0;
};

struct ObjectTag32Big final : ObjectTag<AssetId32Big> {
  ObjectTag32Big() = default;
  void PutTo(athena::io::IStreamWriter& out, bool reversed = false) const override;
  void PutTo(nlohmann::ordered_json& j) const override;
  ObjectTag32Big(const FourCC& type, const AssetId32Big& id) : ObjectTag(type), m_id(id) {}

  template <bool Reverse = false>
  static ObjectTag32Big Load(athena::io::IStreamReader& in) {
    ObjectTag32Big ret;
    if constexpr (!Reverse) {
      ret.type = FourCC(in);
      ret.m_id = AssetId32Big(in, ret.type);
    } else {
      uint32_t id = in.readUint32Big();
      ret.type = FourCC(in);
      ret.m_id = AssetId32Big(id, ret.type);
    }

    return ret;
  }

  static ObjectTag32Big Load(const nlohmann::ordered_json& j);

  AssetId32Big id() const override { return m_id; }
  bool operator<(const ObjectTag32Big& other) const { return m_id < other.m_id; }
  explicit operator bool() const { return type != kInvalidFourCC && m_id != AssetId32Big(); }

  std::string_view repPath() const override { return m_id.repPath(); }

private:
  AssetId32Big m_id;
};

} // namespace axdl::primedep
