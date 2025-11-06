#include "PrimeDep/Animation/POINode.hpp"

#include "magic_enum/magic_enum.hpp"

#include <nlohmann/json.hpp>
namespace axdl::primedep {
POINode::POINode(std::string_view name, EPOIType type, const CharAnimTime& time, int32_t index, bool unique,
                 float weight, int32_t charIdx, int32_t flags)
: x4_(1)
, x8_name(name)
, x18_type(type)
, x1c_time(time)
, x24_index(index)
, x28_unique(unique)
, x2c_weight(weight)
, x30_charIdx(charIdx)
, x34_flags(flags) {}

POINode::POINode(athena::io::IStreamReader& in)
: x4_(in.readUint16Big())
, x8_name(in.readString())
, x18_type(static_cast<EPOIType>(in.readUint16Big()))
, x1c_time(in)
, x24_index(in.readInt32Big())
, x28_unique(in.readBool())
, x2c_weight(in.readFloatBig())
, x30_charIdx(in.readInt32Big())
, x34_flags(in.readInt32Big()) {
  assert(x4_ == 1 && "x4_ is not 1!");
}

POINode::POINode(const nlohmann::ordered_json& in)
: x8_name(in.value("Name", x8_name))
, x18_type(magic_enum::enum_cast<EPOIType>(in.value("Type", "")).value_or(EPOIType::Invalid))
, x1c_time(in["Time"])
, x24_index(in.value("Index", x24_index))
, x28_unique(in.value("Unique", x28_unique))
, x2c_weight(in.value("Weight", x2c_weight))
, x30_charIdx(in.value("Character", x30_charIdx))
, x34_flags(in.value("Flags", x34_flags)) {}

bool POINode::operator>(const POINode& other) const { return x1c_time > other.x1c_time; }

bool POINode::operator<(const POINode& other) const { return x1c_time < other.x1c_time; }

void POINode::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint16Big(x4_);
  out.writeString(x8_name);
  out.writeUint16Big(static_cast<uint16_t>(x18_type));
  x1c_time.PutTo(out);
  out.writeInt32Big(x24_index);
  out.writeBool(x28_unique);
  out.writeFloatBig(x2c_weight);
  out.writeInt32Big(x30_charIdx);
  out.writeInt32Big(x34_flags);
}

void POINode::PutTo(nlohmann::ordered_json& j) const {
  j["Name"] = x8_name;
  j["Type"] = magic_enum::enum_name(x18_type);
  x1c_time.PutTo(j["Time"]);
  j["Index"] = x24_index;
  j["Unique"] = x28_unique;
  j["Weight"] = x2c_weight;
  j["Character"] = x30_charIdx;
  j["Flags"] = x34_flags;
}

} // namespace axdl::primedep