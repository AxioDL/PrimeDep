#include "PrimeDep/Animation/Int32POINode.hpp"
#include <nlohmann/json.hpp>
namespace axdl::primedep {
Int32POINode::Int32POINode()
: Int32POINode(""sv, EPOIType::EmptyInt32, CharAnimTime(), -1, false, 1.f, -1, 0, 0, "root"sv) {}

Int32POINode::Int32POINode(const std::string_view name, const EPOIType type, const CharAnimTime& time,
                           const int32_t index, const bool unique, const float weight, const int32_t charIndex,
                           const int32_t flags, const int32_t val, const std::string_view locator)
: POINode(name, type, time, index, unique, weight, charIndex, flags), x38_val(val), x3c_locatorName(locator) {}

Int32POINode::Int32POINode(athena::io::IStreamReader& in)
: POINode(in), x38_val(in.readInt32Big()), x3c_locatorName(in.readString()) {}

Int32POINode::Int32POINode(const nlohmann::ordered_json& in)
: POINode(in), x38_val(in.value("Value", x38_val)), x3c_locatorName(in.value("Locator", x3c_locatorName)) {}

Int32POINode Int32POINode::CopyNodeMinusStartTime(const Int32POINode& node, const CharAnimTime& startTime) {
  Int32POINode ret = node;
  ret.x1c_time -= startTime;
  return ret;
}

void Int32POINode::PutTo(athena::io::IStreamWriter& out) const {
  POINode::PutTo(out);
  out.writeUint32(x38_val);
  out.writeString(x3c_locatorName);
}
void Int32POINode::PutTo(nlohmann::ordered_json& j) const {
  POINode::PutTo(j);
  j["Value"] = x38_val;
  j["Locator"] = x3c_locatorName;
}

} // namespace axdl::primedep