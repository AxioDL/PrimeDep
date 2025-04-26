#include "PrimeDep/Animation/BoolPOINode.hpp"
#include <nlohmann/json.hpp>

namespace axdl::primedep {
BoolPOINode::BoolPOINode() : POINode("root", EPOIType::EmptyBool, CharAnimTime(), -1, false, 1.f, -1, 0) {}

BoolPOINode::BoolPOINode(athena::io::IStreamReader& in) : POINode(in), x38_val(in.readBool()) {}

BoolPOINode BoolPOINode::CopyNodeMinusStartTime(const BoolPOINode& node, const CharAnimTime& startTime) {
  BoolPOINode ret = node;
  ret.x1c_time -= startTime;
  return ret;
}

void BoolPOINode::PutTo(nlohmann::ordered_json& j) const {
  POINode::PutTo(j);
  j["Value"] = x38_val;
}

} // namespace axdl::primedep