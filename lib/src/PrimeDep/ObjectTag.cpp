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
  const auto res = ResourcePool32Big::instance()->resourceById(*this);
  j["Type"] = type.toString();
  if (res) {
    j["File"] = res->cookedPath(res->repPath());
  }

  if (!res || !res->pathKnown()) {
    j["AssetID"] = id().toString();
  }
}

ObjectTag32Big ObjectTag32Big::Load(const nlohmann::ordered_json& j) {
  ObjectTag32Big ret;
  ret.type = FourCC(j.value("Type", kInvalidFourCC.toString()));
  // If we have an asset id just ignore the filename even if we have one
  if (j.contains("File")) {
    std::string file = j.value("File", "");
    athena::utility::tolower(file);
    ret.m_id = AssetId32Big(CRC32::Calculate(file.c_str(), file.length()));
    ret.m_repPath = file;
  }

  if (j.contains("AssetID")) {
    ret.m_id = AssetId32Big(j.value("AssetID", kInvalidAssetId32Big.toString()));
  }

  if (ret.type == kInvalidFourCC) {
    return {};
  }
  return ret;
}

} // namespace axdl::primedep