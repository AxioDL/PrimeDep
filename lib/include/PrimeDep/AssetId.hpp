#pragma once
#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>
#include <cstdint>

namespace axdl::primedep {
template <typename T, T Default>
class AssetId {
public:
  AssetId() = default;
  explicit AssetId(const T id) : id(id) {}
  virtual ~AssetId() = default;

  virtual void PutTo(athena::io::IStreamWriter& out) const = 0;

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
};

class AssetId32Big final : public AssetId<uint32_t, static_cast<uint32_t>(-1)> {
public:
  explicit AssetId32Big(const uint32_t id = static_cast<uint32_t>(-1)) : AssetId(id) {}
  explicit AssetId32Big(athena::io::IStreamReader& in) : AssetId32Big(in.readUint32Big()) {}
  explicit AssetId32Big(const std::string& str) {
    auto idStr = str;
    if (idStr.starts_with("0x") || idStr.starts_with("0X")) {
      idStr = idStr.substr(2);
    }
    id = strtol(idStr.c_str(), nullptr, 16);
  }
  void PutTo(athena::io::IStreamWriter& out) const override { out.writeUint32Big(id); }
  std::string toString() const override { return std::format("{:08X}", id); }
};

static const AssetId32Big kInvalidAssetId32Big = AssetId32Big();

} // namespace axdl::primedep