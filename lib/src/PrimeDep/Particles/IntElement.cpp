#include "PrimeDep/Particles/IntElement.hpp"

#include "PrimeDep/Particles/ParticleDataFactory.hpp"

#include "PrimeDep/Particles/RealElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {

void IENone::PutTo(athena::io::IStreamWriter& out) const { ParticleDataFactory::SetClassID(out, FOURCC('NONE')); }
void IENone::PutTo(nlohmann::ordered_json& out) const { ParticleDataFactory::SetClassID(out, "None"); }

IEConstant::IEConstant(athena::io::IStreamReader& in) : m_value(ParticleDataFactory::GetInt(in)) {}
IEConstant::IEConstant(const nlohmann::ordered_json& in) : m_value(in.value("Value", 0)) {}

void IEConstant::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('CNST'));
  ParticleDataFactory::SetInt(out, m_value);
}

void IEConstant::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "Constant");
  ParticleDataFactory::SetInt(out, "Value", m_value);
}

IEKeyframeEmitter::IEKeyframeEmitter(athena::io::IStreamReader& in)
: m_percent(in.readUint32Big())
, m_unknown1(in.readUint32Big())
, m_loop(in.readBool())
, m_unknown2(in.readBool())
, m_loopEnd(in.readInt32Big())
, m_loopStart(in.readInt32Big()) {
  int keyCount = in.readUint32Big();
  while (keyCount--) {
    m_keys.emplace_back(in.readInt32Big());
  }
}

IEKeyframeEmitter::IEKeyframeEmitter(const nlohmann::ordered_json& in)
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

void IEKeyframeEmitter::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, m_percent ? FOURCC('KEYP') : FOURCC('KEYE'));
  out.writeUint32Big(m_percent);
  out.writeUint32Big(m_unknown1);
  out.writeBool(m_loop);
  out.writeBool(m_unknown2);
  out.writeInt32Big(m_loopEnd);
  out.writeInt32Big(m_loopStart);
  out.writeUint32Big(m_keys.size());
  for (const auto& key : m_keys) {
    out.writeInt32Big(key);
  }
}

void IEKeyframeEmitter::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "KeyframeEmitter");
  out["Percent"] = m_percent;
  out["Unknown1"] = m_unknown1;
  out["Loop"] = m_loop;
  out["Unknown2"] = m_unknown2;
  out["LoopEnd"] = m_loopEnd;
  out["LoopStart"] = m_loopStart;
  auto& keys = out["Keys"];
  for (const auto& key : m_keys) {
    keys.emplace_back(key);
  }
}

IETimeScale::IETimeScale(athena::io::IStreamReader& in) : m_scale(ParticleDataFactory::GetRealElement(in)) {}
IETimeScale::IETimeScale(const nlohmann::ordered_json& in)
: m_scale(ParticleDataFactory::GetRealElement(in, "Scale")) {}

IETimeScale::~IETimeScale() {}

void IETimeScale::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_scale) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('TSCL'));
  m_scale->PutTo(out);
}

void IETimeScale::PutTo(nlohmann::ordered_json& out) const {
  if (!m_scale) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "TimeScale"sv);
  m_scale->PutTo(out["Scale"]);
}

IEDeath::IEDeath(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetIntElement(in)), m_b(ParticleDataFactory::GetIntElement(in)) {}

IEDeath::IEDeath(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

void IEDeath::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('DETH'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEDeath::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Death");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

IETimeChain::IETimeChain(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetIntElement(in))
, m_b(ParticleDataFactory::GetIntElement(in))
, m_switchFrame(ParticleDataFactory::GetIntElement(in)) {}

IETimeChain::IETimeChain(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetIntElement(in, "A"))
, m_b(ParticleDataFactory::GetIntElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

void IETimeChain::PutTo(athena::io::IStreamWriter& out) const {
  // Int Elements must have all 3 elements specified
  if (!m_a || !m_b || !m_switchFrame) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('CHAN'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void IETimeChain::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_switchFrame) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "TimeChain"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_switchFrame->PutTo(out["SwitchFrame"]);
}

IEAdd::IEAdd(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetIntElement(in)), m_b(ParticleDataFactory::GetIntElement(in)) {}

IEAdd::IEAdd(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

void IEAdd::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('ADD_'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEAdd::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Add");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

IEMultiply::IEMultiply(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetIntElement(in)), m_b(ParticleDataFactory::GetIntElement(in)) {}

IEMultiply::IEMultiply(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

void IEMultiply::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('MULT'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEMultiply::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Multiply");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

IEModulo::IEModulo(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetIntElement(in)), m_b(ParticleDataFactory::GetIntElement(in)) {}

IEModulo::IEModulo(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

void IEModulo::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('MODU'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEModulo::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Modulo");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

IERandom::IERandom(athena::io::IStreamReader& in)
: m_min(ParticleDataFactory::GetIntElement(in)), m_max(ParticleDataFactory::GetIntElement(in)) {}

IERandom::IERandom(const nlohmann::ordered_json& in)
: m_min(ParticleDataFactory::GetIntElement(in, "Min")), m_max(ParticleDataFactory::GetIntElement(in, "Max")) {}

void IERandom::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_min || !m_max) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('RAND'));
  m_min->PutTo(out);
  m_max->PutTo(out);
}

void IERandom::PutTo(nlohmann::ordered_json& out) const {
  if (!m_min || !m_max) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "Random");
  m_min->PutTo(out["Min"]);
  m_max->PutTo(out["Max"]);
}

IEImpulse::IEImpulse(athena::io::IStreamReader& in) : m_impulse(ParticleDataFactory::GetIntElement(in)) {}

IEImpulse::IEImpulse(const nlohmann::ordered_json& in) : m_impulse(ParticleDataFactory::GetIntElement(in, "Value")) {}

void IEImpulse::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_impulse) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('IMPL'));
  m_impulse->PutTo(out);
}

void IEImpulse::PutTo(nlohmann::ordered_json& out) const {
  if (!m_impulse) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Impulse");
  m_impulse->PutTo(out["Value"]);
}

IELifetimePercent::IELifetimePercent(athena::io::IStreamReader& in)
: m_lifetime(ParticleDataFactory::GetIntElement(in)) {}

IELifetimePercent::IELifetimePercent(const nlohmann::ordered_json& in)
: m_lifetime(ParticleDataFactory::GetIntElement(in, "Value")) {}

void IELifetimePercent::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_lifetime) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('ILPT'));
  m_lifetime->PutTo(out);
}

void IELifetimePercent::PutTo(nlohmann::ordered_json& out) const {
  if (!m_lifetime) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "LifetimePercent");
  m_lifetime->PutTo(out["Value"]);
}

IESampleAndHold::IESampleAndHold(athena::io::IStreamReader& in)
: m_sampleSource(ParticleDataFactory::GetIntElement(in))
, m_waitFramesMin(ParticleDataFactory::GetIntElement(in))
, m_waitFramesMax(ParticleDataFactory::GetIntElement(in)) {}

IESampleAndHold::IESampleAndHold(const nlohmann::ordered_json& in)
: m_sampleSource(ParticleDataFactory::GetIntElement(in, "SampleSource"))
, m_waitFramesMin(ParticleDataFactory::GetIntElement(in, "WaitFramesMin"))
, m_waitFramesMax(ParticleDataFactory::GetIntElement(in, "WaitFramesMax")) {}

void IESampleAndHold::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_sampleSource || !m_waitFramesMin || m_waitFramesMax) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('SPAH'));
  m_sampleSource->PutTo(out);
  m_waitFramesMin->PutTo(out);
  m_waitFramesMax->PutTo(out);
}

void IESampleAndHold::PutTo(nlohmann::ordered_json& out) const {
  if (!m_sampleSource || !m_waitFramesMin || m_waitFramesMax) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "SampleAndHold"sv);
  m_sampleSource->PutTo(out["SampleSource"]);
  m_waitFramesMin->PutTo(out["WaitFramesMin"]);
  m_waitFramesMax->PutTo(out["WaitFramesMax"]);
}

IEInitialRandom::IEInitialRandom(athena::io::IStreamReader& in)
: m_min(ParticleDataFactory::GetIntElement(in)), m_max(ParticleDataFactory::GetIntElement(in)) {}

IEInitialRandom::IEInitialRandom(const nlohmann::ordered_json& in)
: m_min(ParticleDataFactory::GetIntElement(in, "Min")), m_max(ParticleDataFactory::GetIntElement(in, "Max")) {}

void IEInitialRandom::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_min || !m_max) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('IRND'));
  m_min->PutTo(out);
  m_max->PutTo(out);
}

void IEInitialRandom::PutTo(nlohmann::ordered_json& out) const {
  if (!m_min || !m_max) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "InitialRandom");
  m_min->PutTo(out["Min"]);
  m_max->PutTo(out["Max"]);
}

IEClamp::IEClamp(athena::io::IStreamReader& in)
: m_min(ParticleDataFactory::GetIntElement(in))
, m_max(ParticleDataFactory::GetIntElement(in))
, m_value(ParticleDataFactory::GetIntElement(in)) {}

IEClamp::IEClamp(const nlohmann::ordered_json& in)
: m_min(ParticleDataFactory::GetIntElement(in, "Min"))
, m_max(ParticleDataFactory::GetIntElement(in, "Max"))
, m_value(ParticleDataFactory::GetIntElement(in, "Value")) {}

void IEClamp::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_min || !m_max || m_value) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('CLMP'));
  m_min->PutTo(out);
  m_max->PutTo(out);
  m_value->PutTo(out);
}

void IEClamp::PutTo(nlohmann::ordered_json& out) const {
  if (!m_min || !m_max || m_value) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Clamp"sv);
  m_min->PutTo(out["Min"]);
  m_max->PutTo(out["Max"]);
  m_value->PutTo(out["Value"]);
}

IEPulse::IEPulse(athena::io::IStreamReader& in)
: m_aDuration(ParticleDataFactory::GetIntElement(in))
, m_bDuration(ParticleDataFactory::GetIntElement(in))
, m_a(ParticleDataFactory::GetIntElement(in))
, m_b(ParticleDataFactory::GetIntElement(in)) {}

IEPulse::IEPulse(const nlohmann::ordered_json& in)
: m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetIntElement(in, "A"))
, m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

IEPulse::~IEPulse() {}

void IEPulse::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_aDuration || !m_bDuration || !m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('PULS'));
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEPulse::PutTo(nlohmann::ordered_json& out) const {
  if (!m_aDuration || !m_bDuration || !m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Pulse"sv);
  m_aDuration->PutTo(out["DurationA"]);
  m_aDuration->PutTo(out["DurationB"]);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

IERealToInt::IERealToInt(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in)), m_b(ParticleDataFactory::GetRealElement(in)) {}

IERealToInt::IERealToInt(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A")), m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

IERealToInt::~IERealToInt() {}

void IERealToInt::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('RTOI'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IERealToInt::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "RealToInt");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

IESubtract::IESubtract(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetIntElement(in)), m_b(ParticleDataFactory::GetIntElement(in)) {}

IESubtract::IESubtract(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

void IESubtract::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('SUB_'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IESubtract::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Subtract");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

void IEGetCumulativeParticleCount::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('GTCP'));
}

void IEGetCumulativeParticleCount::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "GetCumulativeParticleCount"sv);
}

void IEGetActiveParticleCount::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('GAPC'));
}

void IEGetActiveParticleCount::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "GetActiveParticleCount"sv);
}

void IEGetEmitterTime::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('GEMT'));
}

void IEGetEmitterTime::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "GetEmitterTime"sv);
}
} // namespace axdl::primedep::particles