#pragma once
#include "athena/IStreamReader.hpp"
#include "PrimeDep/Utils.hpp"

namespace axdl::primedep {
class FourCC {
public:
  constexpr FourCC() noexcept = default;
  constexpr FourCC(const FourCC&) noexcept = default;
  constexpr FourCC(FourCC&&) noexcept = default;
  constexpr explicit FourCC(const char name[4]) noexcept : fcc{name[0], name[1], name[2], name[3]} {}
  constexpr explicit FourCC(const uint32_t n) noexcept : num(n) {}
  constexpr explicit FourCC(const std::string_view name) noexcept {
    if (name.length() < 4) {
      return;
    }
    for (int i = 0; i < 4; ++i) {
      fcc[i] = name[i];
    }
  }

  explicit FourCC(athena::io::IStreamReader& in) { in.readUBytesToBuf(fcc, 4); }

  constexpr FourCC& operator=(const FourCC&) noexcept = default;
  constexpr FourCC& operator=(FourCC&&) noexcept = default;

  [[nodiscard]] std::string toString() const noexcept { return std::format("{}{}{}{}", fcc[0], fcc[1], fcc[2], fcc[3]); }

  bool operator==(const FourCC& rhs) const noexcept { return num == rhs.num; }
  bool operator!=(const FourCC& rhs) const noexcept { return num != rhs.num; }
  bool operator<(const FourCC& rhs) const noexcept { return num < rhs.num; }
  bool operator>(const FourCC& rhs) const noexcept { return num > rhs.num; }
  bool operator<=(const FourCC& rhs) const noexcept { return num <= rhs.num; }
  bool operator>=(const FourCC& rhs) const noexcept { return num >= rhs.num; }

  explicit operator bool() const noexcept { return num != 0; }

  union {
    char fcc[4];
    uint32_t num = 0;
  };
};

static constexpr FourCC kInvalidFourCC = {};

} // namespace axdl::primedep