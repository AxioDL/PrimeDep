#include "PrimeDep/Particles/ParticleData.hpp"
#include "PrimeDep/ResourceUtils.hpp"

#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>

namespace axdl::primedep {
// Taken from metaforce
ParticleData::ParticleData(athena::io::IStreamReader& in)
: x0_duration(in.readUint32Big())
, x4_particle(ObjectTag32Big::Load<false>(in))
, xc_boneName(in.readString())
, x1c_scale(in.readFloatBig())
, x20_parentMode(static_cast<EParentedMode>(in.readUint32Big())) {}

ParticleData::ParticleData(const nlohmann::ordered_json& in) {
  x0_duration = in.value("Duration", x0_duration);

  if (in.contains("Ref")) {
    x4_particle = ObjectTag32Big::Load(in["Ref"]);
  }
  xc_boneName = in.value("BoneName", xc_boneName);
  x1c_scale = in.value("Scale", x1c_scale);

  x20_parentMode = magic_enum::enum_cast<EParentedMode>(in.value("ParentMode", "")).value_or(EParentedMode::Initial);
}

void ParticleData::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(x0_duration);
  x4_particle.PutTo(out);
  out.writeString(xc_boneName);
  out.writeFloatBig(x1c_scale);
  out.writeUint32Big(static_cast<uint32_t>(x20_parentMode));
}

void ParticleData::PutTo(nlohmann::ordered_json& j) const {
  j["Duration"] = x0_duration;
  x4_particle.PutTo(j["Ref"]);
  j["BoneName"] = xc_boneName;
  j["Scale"] = x1c_scale;
  j["ParentMode"] = magic_enum::enum_name(x20_parentMode);
}

} // namespace axdl::primedep