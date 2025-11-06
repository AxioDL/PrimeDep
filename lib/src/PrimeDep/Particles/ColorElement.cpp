#include "PrimeDep/Particles/ColorElement.hpp"

#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
CEConstant::CEConstant(athena::io::IStreamReader& in) {
  m_r.reset(ParticleDataFactory::GetRealElement(in));
  m_g.reset(ParticleDataFactory::GetRealElement(in));
  m_b.reset(ParticleDataFactory::GetRealElement(in));
  m_a.reset(ParticleDataFactory::GetRealElement(in));
}
CEConstant::CEConstant(nlohmann::ordered_json& in) {}

void CEConstant::PutTo(athena::io::IStreamWriter& out) const {
  // Color Elements must have all 4 channels specified
  if (!m_r || !m_g || !m_b || !m_a) {
    return;
  }

  // Write the class identifier first
  FOURCC('CNST').PutTo(out);

  // Now the color channels in RGBA order
  m_r->PutTo(out);
  m_g->PutTo(out);
  m_b->PutTo(out);
  m_a->PutTo(out);
}

void CEConstant::PutTo(nlohmann::ordered_json& out) const {
  // Color Elements must have all 4 channels specified
  if (!m_r || !m_g || !m_b || !m_a) {
    return;
  }

  out["Type"] = "ColorConstant";
  m_r->PutTo(out["R"]);
  m_r->PutTo(out["G"]);
  m_r->PutTo(out["B"]);
  m_r->PutTo(out["A"]);
}

CEKeyframeEmitter::CEKeyframeEmitter(athena::io::IStreamReader& in)
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
  assert(m_isKeyP == m_percent);
}

CEKeyframeEmitter::CEKeyframeEmitter(const nlohmann::ordered_json& in)
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

void CEKeyframeEmitter::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(m_percent);
  out.writeUint32Big(m_unknown1);
  out.writeBool(m_loop);
  out.writeBool(m_unknown2);
  out.writeBool(m_loopEnd);
  out.writeInt32Big(m_loopStart);
  out.writeUint32Big(m_keys.size());
  for (const auto& key : m_keys) {
    key.PutTo(out);
  }
}
void CEKeyframeEmitter::PutTo(nlohmann::ordered_json& out) const {
  out["Type"] = "ColorKeyframeEmitter";
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

} // namespace axdl::primedep::particles