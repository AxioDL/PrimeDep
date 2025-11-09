#include "PrimeDep/Particles/ColorElement.hpp"

#include "PrimeDep/Particles/ParticleDataFactory.hpp"

#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ParticleData.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
void CENone::PutTo(athena::io::IStreamWriter& out) const { ParticleDataFactory::SetClassID(out, FOURCC('NONE')); }
void CENone::PutTo(nlohmann::ordered_json& out) const { ParticleDataFactory::SetClassID(out, "None"); }
CEConstant::CEConstant(athena::io::IStreamReader& in)
: m_r(ParticleDataFactory::GetRealElement(in))
, m_g(ParticleDataFactory::GetRealElement(in))
, m_b(ParticleDataFactory::GetRealElement(in))
, m_a(ParticleDataFactory::GetRealElement(in)) {}

CEConstant::CEConstant(const nlohmann::ordered_json& in) {}

CEConstant::~CEConstant() {}

void CEConstant::PutTo(athena::io::IStreamWriter& out) const {
  // Color Elements must have all 4 channels specified
  if (!m_r || !m_g || !m_b || !m_a) {
    return;
  }

  // Write the class identifier first
  ParticleDataFactory::SetClassID(out, FOURCC('CNST'));

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

  ParticleDataFactory::SetClassID(out, "Constant");
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

void CEKeyframeEmitter::PutTo(nlohmann::ordered_json& out) const {
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

CETimeChain::CETimeChain(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetColorElement(in))
, m_b(ParticleDataFactory::GetColorElement(in))
, m_switchFrame(ParticleDataFactory::GetIntElement(in)) {}

CETimeChain::CETimeChain(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

CETimeChain::~CETimeChain() {}

void CETimeChain::PutTo(athena::io::IStreamWriter& out) const {
  // Color Elements must have all 3 elements specified
  if (!m_a || !m_b || !m_switchFrame) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('CHAN'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void CETimeChain::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_switchFrame) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "TimeChain"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_switchFrame->PutTo(out["SwitchFrame"]);
}

CEFadeEnd::CEFadeEnd(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetColorElement(in))
, m_b(ParticleDataFactory::GetColorElement(in))
, m_startFrame(ParticleDataFactory::GetRealElement(in))
, m_endFrame(ParticleDataFactory::GetRealElement(in)) {}

CEFadeEnd::CEFadeEnd(const nlohmann::ordered_json& in) {}

CEFadeEnd::~CEFadeEnd() {}

void CEFadeEnd::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b || !m_startFrame || !m_endFrame) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('CFDE'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_startFrame->PutTo(out);
  m_endFrame->PutTo(out);
}

void CEFadeEnd::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_startFrame || !m_endFrame) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "FadeEnd"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_startFrame->PutTo(out["StartFrame"]);
  m_endFrame->PutTo(out["EndFrame"]);
}

CEFade::CEFade(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetColorElement(in))
, m_b(ParticleDataFactory::GetColorElement(in))
, m_endFrame(ParticleDataFactory::GetRealElement(in)) {}

CEFade::CEFade(const nlohmann::ordered_json& in) {}

CEFade::~CEFade() {}

void CEFade::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b || !m_endFrame) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('FADE'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_endFrame->PutTo(out);
}

void CEFade::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_endFrame) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Fade"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_endFrame->PutTo(out["EndFrame"]);
}

CEPulse::CEPulse(athena::io::IStreamReader& in)
: m_aDuration(ParticleDataFactory::GetIntElement(in))
, m_bDuration(ParticleDataFactory::GetIntElement(in))
, m_a(ParticleDataFactory::GetColorElement(in))
, m_b(ParticleDataFactory::GetColorElement(in)) {}

CEPulse::CEPulse(const nlohmann::ordered_json& in)
: m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B")) {}

CEPulse::~CEPulse() {}

void CEPulse::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_aDuration || !m_bDuration || !m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('PULS'));
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void CEPulse::PutTo(nlohmann::ordered_json& out) const {
  if (!m_aDuration || !m_bDuration || !m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Pulse"sv);
  m_aDuration->PutTo(out["DurationA"]);
  m_aDuration->PutTo(out["DurationB"]);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}
CEParticleColor::CEParticleColor(athena::io::IStreamReader& in) {}
CEParticleColor::CEParticleColor(const nlohmann::ordered_json& in) {}

void CEParticleColor::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PCOL'));
}

void CEParticleColor::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleColor");
}

} // namespace axdl::primedep::particles