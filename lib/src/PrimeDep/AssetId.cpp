#include "PrimeDep/AssetId.hpp"

#include "PrimeDep/CRC32.hpp"
#include "PrimeDep/ResourceNameDatabase.hpp"
#include "PrimeDep/ResourcePool.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep {
AssetId32Big::AssetId32Big(const nlohmann::ordered_json& in, const FourCC& type) {
  if (!in.contains("File") && !in.contains("AssetId")) {
    return;
  }

  m_repPath = in.value("File", "");
  m_known = !in.contains("AssetID");
  if (in.contains("AssetID")) {
    id = in["AssetID"].get<uint32_t>();
  } else {
    auto lowerPath = ResourcePool32Big::instance()->cookedRepPathFromRawRepPath(m_repPath, type);
    athena::utility::tolower(lowerPath);
    id = CRC32::Calculate(lowerPath.c_str(), lowerPath.length());
  }
}

void AssetId32Big::PutTo(athena::io::IStreamWriter& out) const { out.writeUint32Big(id); }
void AssetId32Big::PutTo(nlohmann::ordered_json& out, const FourCC& type) const {
  if (*this == kInvalidAssetId32Big) {
    return;
  }

  if (!m_repPath.empty()) {
    out["File"] = ResourcePool32Big::instance()->rawRepPathFromCookedRepPath(m_repPath, type);
  }
  if (!m_known) {
    out["AssetID"] = id;
  }
}

void AssetId32Big::resolveRepPath(const FourCC& fcc) {
  m_known = ResourceNameDatabase::instance().hasPath(ObjectTag32Big(fcc, *this));
  m_repPath = ResourceNameDatabase::instance().pathForAsset(ObjectTag32Big(fcc, *this));
}

} // namespace axdl::primedep