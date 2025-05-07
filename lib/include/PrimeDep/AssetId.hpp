#pragma once

#include "PrimeDep/FourCC.hpp"

#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>
#include <cstdint>
#include <nlohmann/json_fwd.hpp>

namespace axdl::primedep {
template <typename T, T Default>
class AssetId {
public:
  AssetId() = default;
  explicit AssetId(const T id) : id(id) {}
  virtual ~AssetId() = default;

  virtual void PutTo(athena::io::IStreamWriter& out) const = 0;
  virtual void PutTo(nlohmann::ordered_json& out, const FourCC& type) const = 0;

  T id = Default;

  bool operator==(const AssetId& rhs) const { return id == rhs.id; }
  bool operator!=(const AssetId& rhs) const { return id != rhs.id; }
  bool operator<(const AssetId& rhs) const { return id < rhs.id; }
  bool operator>(const AssetId& rhs) const { return id > rhs.id; }
  bool operator<=(const AssetId& rhs) const { return id <= rhs.id; }
  bool operator>=(const AssetId& rhs) const { return id >= rhs.id; }

  explicit operator T() const { return id; }
  explicit operator bool() const { return id != Default; }

  virtual std::string toString() const = 0;
  std::string_view repPath() const { return m_repPath; }
  bool pathKnown() const { return m_known; }

protected:
  std::string m_repPath;
  bool m_known{false};
};

class AssetId32Big final : public AssetId<uint32_t, static_cast<uint32_t>(-1)> {
public:
  AssetId32Big() = default;
  explicit AssetId32Big(const uint32_t id) : AssetId(id) {}
  explicit AssetId32Big(const uint32_t id, const FourCC& fcc) : AssetId(id) { resolveRepPath(fcc); }
  explicit AssetId32Big(athena::io::IStreamReader& in, const FourCC& fcc) : AssetId32Big(in.readUint32Big(), fcc) {
    resolveRepPath(fcc);
  }
  explicit AssetId32Big(const nlohmann::ordered_json& in, const FourCC& type);
  static AssetId32Big FromString(const std::string& str) {
    AssetId32Big ret;
    auto idStr = str;
    if (idStr.starts_with("0x") || idStr.starts_with("0X")) {
      idStr = idStr.substr(2);
    }
    ret.id = strtol(idStr.c_str(), nullptr, 16);
    return ret;
  }

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out, const FourCC& type) const override;
  [[nodiscard]] std::string toString() const override { return std::format("{:08X}", id); }

private:
  void resolveRepPath(const FourCC& fcc);
};

static const auto kInvalidAssetId32Big = AssetId32Big();

} // namespace axdl::primedep