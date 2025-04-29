#include "PrimeDep/ObjectTag.hpp"

#include "PrimeDep/ResourcePool.hpp"

namespace axdl::primedep {
void ObjectTag32Big::PutTo(athena::io::IStreamWriter& out, const bool reversed) const {
  if (!reversed) {
    out.writeBytes(type.fcc, sizeof(type.fcc));
    id().PutTo(out);
  } else {
    id().PutTo(out);
    out.writeBytes(type.fcc, sizeof(type.fcc));
  }
}
void ObjectTag32Big::PutTo(nlohmann::ordered_json& j) const {
  if (id() == kInvalidAssetId32Big && m_repPath.empty()) {
    return;
  }

  const auto res = ResourcePool32Big::instance()->resourceById(*this);
  j["Type"] = type.toString();
  id().PutTo(j["Ref"]);
}

ObjectTag32Big ObjectTag32Big::Load(const nlohmann::ordered_json& j) {
  ObjectTag32Big ret;
  ret.type = FourCC(j.value("Type", kInvalidFourCC.toString()));
  ret.m_id = AssetId32Big(j["Ref"]);

  return ret;
}

} // namespace axdl::primedep