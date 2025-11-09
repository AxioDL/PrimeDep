#include "PrimeDep/Particles/VectorElement.hpp"

#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ParticleData.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/RealElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
void VENone::PutTo(athena::io::IStreamWriter& out) const { ParticleDataFactory::SetClassID(out, FOURCC('NONE')); }
void VENone::PutTo(nlohmann::ordered_json& out) const { ParticleDataFactory::SetClassID(out, "None"); }

VEConstant::VEConstant(athena::io::IStreamReader& in)
: m_x(ParticleDataFactory::GetRealElement(in))
, m_y(ParticleDataFactory::GetRealElement(in))
, m_z(ParticleDataFactory::GetRealElement(in)) {}

VEConstant::VEConstant(const nlohmann::ordered_json& in)
: m_x(ParticleDataFactory::GetRealElement(in, "X"))
, m_y(ParticleDataFactory::GetRealElement(in, "Y"))
, m_z(ParticleDataFactory::GetRealElement(in, "Z")) {}

VEConstant::~VEConstant() {}

void VEConstant::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_x || !m_y || !m_z) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('CNST'));
  m_x->PutTo(out);
  m_y->PutTo(out);
  m_z->PutTo(out);
}

void VEConstant::PutTo(nlohmann::ordered_json& out) const {
  if (!m_x || !m_y || !m_z) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Constant");
  m_x->PutTo(out["X"]);
  m_y->PutTo(out["Y"]);
  m_z->PutTo(out["Z"]);
}

VEKeyframeEmitter::VEKeyframeEmitter(athena::io::IStreamReader& in)
: m_percent(in.readUint32Big())
, m_unknown1(in.readUint32Big())
, m_loop(in.readBool())
, m_unknown2(in.readBool())
, m_loopEnd(in.readInt32Big())
, m_loopStart(in.readInt32Big()) {
  int keyCount = in.readUint32Big();
  while (keyCount--) {
    m_keys.emplace_back(in);
  }
}

VEKeyframeEmitter::VEKeyframeEmitter(const nlohmann::ordered_json& in)
: m_percent(in.value("Percent", 0))
, m_unknown1(in.value("Unknown1", 0))
, m_loop(in.value("Loop", false))
, m_unknown2(in.value("Unknown2", false))
, m_loopEnd(in.value("LoopEnd", 0))
, m_loopStart(in.value("LoopStart", 0)) {
  if (in.contains("Keys")) {
    for (const auto& keys = in["Keys"]; const auto& key : keys) {
      m_keys.emplace_back(key);
    }
  }
}

void VEKeyframeEmitter::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, m_percent ? FOURCC('KEYP') : FOURCC('KEYE'));
  out.writeUint32Big(m_percent);
  out.writeUint32Big(m_unknown1);
  out.writeBool(m_loop);
  out.writeBool(m_unknown2);
  out.writeInt32Big(m_loopEnd);
  out.writeInt32Big(m_loopStart);
  out.writeUint32Big(m_keys.size());
  for (const auto& key : m_keys) {
    key.PutTo(out);
  }
}

void VEKeyframeEmitter::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "KeyframeEmitter");
  out["Percent"] = m_percent;
  out["Unknown1"] = m_unknown1;
  out["Loop"] = m_loop;
  out["Unknown2"] = m_unknown2;
  out["LoopEnd"] = m_loopEnd;
  out["LoopStart"] = m_loopStart;
  auto& keys = out["Keys"];
  for (const auto& key : m_keys) {
    key.PutTo(keys.emplace_back());
  }
}

VETimeChain::VETimeChain(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetVectorElement(in))
, m_b(ParticleDataFactory::GetVectorElement(in))
, m_switchFrame(ParticleDataFactory::GetIntElement(in)) {}

VETimeChain::VETimeChain(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

VETimeChain::~VETimeChain() {}

void VETimeChain::PutTo(athena::io::IStreamWriter& out) const {
  // Color Elements must have all 3 elements specified
  if (!m_a || !m_b || !m_switchFrame) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('CHAN'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void VETimeChain::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_switchFrame) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "TimeChain"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_switchFrame->PutTo(out["SwitchFrame"]);
}

VEBounce::VEBounce(athena::io::IStreamReader& in)
: m_planePoint(ParticleDataFactory::GetVectorElement(in))
, m_planeNormal(ParticleDataFactory::GetVectorElement(in))
, m_friction(ParticleDataFactory::GetRealElement(in))
, m_restitution(ParticleDataFactory::GetRealElement(in))
, m_dieOnPenetration(ParticleDataFactory::GetBool(in)) {}

VEBounce::VEBounce(const nlohmann::ordered_json& in)
: m_planePoint(ParticleDataFactory::GetVectorElement(in, "PlanePoint"))
, m_planeNormal(ParticleDataFactory::GetVectorElement(in, "PlaneNormal"))
, m_friction(ParticleDataFactory::GetRealElement(in, "Friction"))
, m_restitution(ParticleDataFactory::GetRealElement(in, "Restitution"))
, m_dieOnPenetration(ParticleDataFactory::GetBool(in, "DieOnPenetration")) {}

VEBounce::~VEBounce() {}

void VEBounce::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_planePoint || !m_planeNormal || !m_friction || !m_restitution) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('BNCE'));
  m_planePoint->PutTo(out);
  m_planeNormal->PutTo(out);
  m_friction->PutTo(out);
  m_restitution->PutTo(out);
  ParticleDataFactory::SetBool(out, m_dieOnPenetration);
}

void VEBounce::PutTo(nlohmann::ordered_json& out) const {
  if (!m_planePoint || !m_planeNormal || !m_friction || !m_restitution) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Bounce");
  m_planePoint->PutTo(out["PlanePoint"]);
  m_planeNormal->PutTo(out["PlaneNormal"]);
  m_friction->PutTo(out["Friction"]);
  m_restitution->PutTo(out["Restitution"]);
  ParticleDataFactory::SetBool(out, "DieOnPenetration", m_dieOnPenetration);
}
} // namespace axdl::primedep::particles