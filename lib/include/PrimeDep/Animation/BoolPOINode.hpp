#pragma once

#include "PrimeDep/Animation/POINode.hpp"

namespace axdl::primedep {
class BoolPOINode final : public POINode {
public:
  explicit BoolPOINode();
  explicit BoolPOINode(athena::io::IStreamReader& in);
  [[nodiscard]] bool GetValue() const { return x38_val; }
  static BoolPOINode CopyNodeMinusStartTime(const BoolPOINode& node, const CharAnimTime& startTime);

  void PutTo(nlohmann::ordered_json& j) const override;
  
private:
  bool x38_val;
};
} // namespace axdl::primedep