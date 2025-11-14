#pragma once
#include "PrimeDep/Types.hpp"

namespace axdl::primedep::MetroidPrime {
struct EditorId {
  uint32_t id = UINT32_MAX;

  constexpr EditorId() noexcept = default;
  explicit constexpr EditorId(const uint32_t idin) noexcept : id(idin) {}
  [[nodiscard]] constexpr uint8_t LayerNum() const noexcept { return static_cast<uint8_t>((id >> 26) & 0x3f); }
  [[nodiscard]] constexpr uint16_t AreaNum() const noexcept { return static_cast<uint16_t>((id >> 16) & 0x3ff); }
  [[nodiscard]] constexpr uint16_t Id() const noexcept { return static_cast<uint16_t>(id & 0xffff); }
  [[nodiscard]] bool operator<(const EditorId& other) const { return (id & 0x3ffffff) < (other.id & 0x3ffffff); }
  [[nodiscard]] bool operator==(const EditorId& other) const { return (id & 0x3ffffff) == (other.id & 0x3ffffff); }
  [[nodiscard]] bool operator!=(const EditorId& other) const { return !(*this == (other)); }

  explicit constexpr operator uint32_t() const noexcept { return id; }
};

static constexpr EditorId kInvalidEditorId;
} // namespace axdl::primedep::MetroidPrime