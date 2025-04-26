#pragma once
#include "PrimeDep/ResourceNameDatabase.hpp"

namespace axdl::primedep {
template <typename ResourceType, typename T, bool BigEndian>
void PutTo(const ObjectTag<T, BigEndian>& tag, nlohmann::ordered_json& j) {
  j["Type"] = tag.type.toString();
  j["File"] = ResourceType::GetRawPath(ResourceNameDatabase::instance().pathForAsset(tag)).generic_string();
}

} // namespace axdl::primedep