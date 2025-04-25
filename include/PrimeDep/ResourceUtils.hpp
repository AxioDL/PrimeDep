#pragma once
#include "PrimeDep/ResourceNameDatabase.hpp"

namespace axdl::primedep {
template <typename T, bool BigEndian>
void PutTo(const ObjectTag<T, BigEndian>& tag, nlohmann::ordered_json& j) {
  j["Type"] = tag.type.toString();
  j["File"] = ResourceNameDatabase::instance().pathForAsset(tag);
}

} // namespace axdl::primedep