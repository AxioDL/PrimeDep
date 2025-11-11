#include "PrimeDep/Particles/ColorElement.hpp"

#include "PrimeDep/Particles/ParticleDataFactory.hpp"

#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ParticleData.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {

CEConstant::CEConstant(athena::io::IStreamReader& in)
: ColorElement(in)
, m_r(ParticleDataFactory::GetRealElement(in, "R"))
, m_g(ParticleDataFactory::GetRealElement(in, "G"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_a(ParticleDataFactory::GetRealElement(in, "A")) {}

CEConstant::CEConstant(const nlohmann::ordered_json& in)
: ColorElement(in)
, m_r(ParticleDataFactory::GetRealElement(in, "R"))
, m_g(ParticleDataFactory::GetRealElement(in, "G"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_a(ParticleDataFactory::GetRealElement(in, "A")) {}

CEConstant::~CEConstant() {}

void CEConstant::PutToInternal(athena::io::IStreamWriter& out) const {
  m_r->PutTo(out);
  m_g->PutTo(out);
  m_b->PutTo(out);
  m_a->PutTo(out);
}

void CEConstant::PutToInternal(nlohmann::ordered_json& out) const {
  m_r->PutTo(out);
  m_g->PutTo(out);
  m_b->PutTo(out);
  m_a->PutTo(out);
}

CEKeyframeEmitter::CEKeyframeEmitter(athena::io::IStreamReader& in)
: ColorElement(in)
, m_percent(in.readUint32Big())
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
: ColorElement(in)
, m_percent(in.value("Percent", 0))
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

void CEKeyframeEmitter::PutToInternal(athena::io::IStreamWriter& out) const {
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

void CEKeyframeEmitter::PutToInternal(nlohmann::ordered_json& out) const {
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
: ColorElement(in)
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

CETimeChain::CETimeChain(const nlohmann::ordered_json& in)
: ColorElement(in)
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

CETimeChain::~CETimeChain() {}

void CETimeChain::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void CETimeChain::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

CEFadeEnd::CEFadeEnd(athena::io::IStreamReader& in)
: ColorElement(in)
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B"))
, m_startFrame(ParticleDataFactory::GetRealElement(in, "StartFrame"))
, m_endFrame(ParticleDataFactory::GetRealElement(in, "EndFrame")) {}

CEFadeEnd::CEFadeEnd(const nlohmann::ordered_json& in)
: ColorElement(in)
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B"))
, m_startFrame(ParticleDataFactory::GetRealElement(in, "StartFrame"))
, m_endFrame(ParticleDataFactory::GetRealElement(in, "EndFrame")) {}

CEFadeEnd::~CEFadeEnd() {}

void CEFadeEnd::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_startFrame->PutTo(out);
  m_endFrame->PutTo(out);
}

void CEFadeEnd::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_startFrame->PutTo(out);
  m_endFrame->PutTo(out);
}

CEFade::CEFade(athena::io::IStreamReader& in)
: ColorElement(in)
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B"))
, m_endFrame(ParticleDataFactory::GetRealElement(in, "EndFrame")) {}

CEFade::CEFade(const nlohmann::ordered_json& in)
: ColorElement(in)
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B"))
, m_endFrame(ParticleDataFactory::GetRealElement(in, "EndFrame")) {}

CEFade::~CEFade() {}

void CEFade::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_endFrame->PutTo(out);
}

void CEFade::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_endFrame->PutTo(out);
}

CEPulse::CEPulse(athena::io::IStreamReader& in)
: ColorElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B")) {}

CEPulse::CEPulse(const nlohmann::ordered_json& in)
: ColorElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetColorElement(in, "A"))
, m_b(ParticleDataFactory::GetColorElement(in, "B")) {}

CEPulse::~CEPulse() {}

void CEPulse::PutToInternal(athena::io::IStreamWriter& out) const {
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void CEPulse::PutToInternal(nlohmann::ordered_json& out) const {
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

} // namespace axdl::primedep::particles