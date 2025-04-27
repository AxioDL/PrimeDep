#include "PrimeDep/ObjectTag.hpp"

#include "PrimeDep/ResourcePool.hpp"

namespace axdl::primedep {
void ObjectTag32Big::PutTo(nlohmann::ordered_json& j) const {
  const auto res = ResourcePool32Big::instance()->resourceById(*this);
  j["Type"] = type.toString();
  if (res) {
    j["File"] = res->rawPath(res->repPath());
  } else {
    j["AssetID"] = id.toString();
  }
}

} // namespace axdl::primedep