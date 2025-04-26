#pragma once

#include "PrimeDep/Animation/POINode.hpp"

namespace axdl::primedep {
class Int32POINode final : public POINode {
public:
  Int32POINode();
  Int32POINode(std::string_view name, EPOIType type, const CharAnimTime& time, int32_t index, bool unique, float weight,
               int32_t charIndex, int32_t flags, int32_t val, std::string_view locator);
  explicit Int32POINode(athena::io::IStreamReader& in);
  [[nodiscard]] int32_t GetValue() const { return x38_val; }
  [[nodiscard]] std::string_view GetLocatorName() const { return x3c_locatorName; }

  static Int32POINode CopyNodeMinusStartTime(const Int32POINode& node, const CharAnimTime& startTime);

  void PutTo(nlohmann::ordered_json& j) const override;

private:
  int32_t x38_val;
  std::string x3c_locatorName;
};
} // namespace axdl::primedep