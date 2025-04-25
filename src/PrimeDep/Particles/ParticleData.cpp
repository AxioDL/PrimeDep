#include "PrimeDep/Particles/ParticleData.hpp"
#include "PrimeDep/ResourceUtils.hpp"
#include <nlohmann/json.hpp>
#include <magic_enum/magic_enum.hpp>

namespace axdl::primedep {
ParticleData::ParticleData(athena::io::IStreamReader& in)
: x0_duration(in.readUint32Big())
, x4_particle(ObjectTag32Big::Load<false>(in))
, xc_boneName(in.readString())
, x1c_scale(in.readFloatBig())
, x20_parentMode(static_cast<EParentedMode>(in.readUint32Big())) {}

void ParticleData::PutTo(nlohmann::ordered_json& j) const {
  j["Duration"] = x0_duration;
  axdl::primedep::PutTo(x4_particle, j["Ref"]);
  j["BoneName"] = xc_boneName;
  j["Scale"] = x1c_scale;
  j["ParentMode"] = magic_enum::enum_name(x20_parentMode);
}

} // namespace axdl::primedep