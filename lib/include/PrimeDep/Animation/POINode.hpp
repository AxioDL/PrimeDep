#pragma once
#include "nlohmann/json_fwd.hpp"

#include <cstdint>
#include <string_view>
#include <PrimeDep/Animation/CharAnimTime.hpp>
using namespace std::string_view_literals;

namespace axdl::primedep {
enum class EPOIType : uint16_t {
  Loop = 0,
  EmptyBool = 1,
  EmptyInt32 = 2,
  SoundInt32 = 4,
  Particle = 5,
  UserEvent = 6,
  RandRate = 7,
  Sound = 8,
};

class POINode {
public:
  explicit POINode(std::string_view name, EPOIType type, const CharAnimTime& time, int32_t index, bool unique,
                   float weight, int32_t charIdx, int32_t flags);
  explicit POINode(athena::io::IStreamReader& in);
  virtual ~POINode() = default;

  [[nodiscard]] std::string_view GetString() const { return x8_name; }
  [[nodiscard]] const CharAnimTime& GetTime() const { return x1c_time; }
  void SetTime(const CharAnimTime& time) { x1c_time = time; }
  [[nodiscard]] EPOIType GetPoiType() const { return x18_type; }
  [[nodiscard]] int32_t GetIndex() const { return x24_index; }
  [[nodiscard]] bool GetUnique() const { return x28_unique; }
  [[nodiscard]] float GetWeight() const { return x2c_weight; }
  [[nodiscard]] int32_t GetCharacterIndex() const { return x30_charIdx; }
  [[nodiscard]] int32_t GetFlags() const { return x34_flags; }

  bool operator>(const POINode& other) const;
  bool operator<(const POINode& other) const;

  virtual void PutTo(nlohmann::ordered_json& j) const;

protected:
  uint16_t x4_ = 1;
  std::string x8_name;
  EPOIType x18_type;
  CharAnimTime x1c_time;
  int32_t x24_index;
  bool x28_unique;
  float x2c_weight;
  int32_t x30_charIdx = -1;
  int32_t x34_flags;
};
} // namespace axdl::primedep