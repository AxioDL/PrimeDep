#include "PrimeDep/Particles/IntElement.hpp"

#include "PrimeDep/Particles/ParticleDataFactory.hpp"

#include "PrimeDep/Particles/RealElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {

IEConstant::IEConstant(athena::io::IStreamReader& in) : IntElement(in), m_value(ParticleDataFactory::GetInt(in)) {}
IEConstant::IEConstant(const nlohmann::ordered_json& in) : IntElement(in), m_value(in.value("Value", 0)) {}

void IEConstant::PutToInternal(athena::io::IStreamWriter& out) const { ParticleDataFactory::SetInt(out, m_value); }

void IEConstant::PutToInternal(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetInt(out, "Value", m_value);
}

IEKeyframeEmitter::IEKeyframeEmitter(athena::io::IStreamReader& in)
: IntElement(in)
, m_percent(in.readUint32Big())
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
: IntElement(in)
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

void IEKeyframeEmitter::PutToInternal(athena::io::IStreamWriter& out) const {
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

void IEKeyframeEmitter::PutToInternal(nlohmann::ordered_json& out) const {
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

IETimeScale::IETimeScale(athena::io::IStreamReader& in)
: IntElement(in), m_scale(ParticleDataFactory::GetRealElement(in, "Scale")) {}
IETimeScale::IETimeScale(const nlohmann::ordered_json& in)
: IntElement(in), m_scale(ParticleDataFactory::GetRealElement(in, "Scale")) {}

IETimeScale::~IETimeScale() {}

void IETimeScale::PutToInternal(athena::io::IStreamWriter& out) const { m_scale->PutTo(out); }

void IETimeScale::PutToInternal(nlohmann::ordered_json& out) const { m_scale->PutTo(out); }

IEDeath::IEDeath(athena::io::IStreamReader& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

IEDeath::IEDeath(const nlohmann::ordered_json& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

void IEDeath::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEDeath::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

IETimeChain::IETimeChain(athena::io::IStreamReader& in)
: IntElement(in)
, m_a(ParticleDataFactory::GetIntElement(in, "A"))
, m_b(ParticleDataFactory::GetIntElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

IETimeChain::IETimeChain(const nlohmann::ordered_json& in)
: IntElement(in)
, m_a(ParticleDataFactory::GetIntElement(in, "A"))
, m_b(ParticleDataFactory::GetIntElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

void IETimeChain::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void IETimeChain::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

IEAdd::IEAdd(athena::io::IStreamReader& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

IEAdd::IEAdd(const nlohmann::ordered_json& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

void IEAdd::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEAdd::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

IEMultiply::IEMultiply(athena::io::IStreamReader& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {
  m_a->setPropertyName("A");
  m_b->setPropertyName("B");
}

IEMultiply::IEMultiply(const nlohmann::ordered_json& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {
  m_a->setPropertyName("A");
  m_b->setPropertyName("B");
}

void IEMultiply::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEMultiply::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

IEModulo::IEModulo(athena::io::IStreamReader& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

IEModulo::IEModulo(const nlohmann::ordered_json& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {
  m_a->setPropertyName("A");
  m_b->setPropertyName("B");
}

void IEModulo::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEModulo::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

IERandom::IERandom(athena::io::IStreamReader& in)
: IntElement(in)
, m_min(ParticleDataFactory::GetIntElement(in, "Min"))
, m_max(ParticleDataFactory::GetIntElement(in, "Max")) {}

IERandom::IERandom(const nlohmann::ordered_json& in)
: IntElement(in)
, m_min(ParticleDataFactory::GetIntElement(in, "Min"))
, m_max(ParticleDataFactory::GetIntElement(in, "Max")) {}

void IERandom::PutToInternal(athena::io::IStreamWriter& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
}

void IERandom::PutToInternal(nlohmann::ordered_json& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
}

IEImpulse::IEImpulse(athena::io::IStreamReader& in)
: IntElement(in), m_impulse(ParticleDataFactory::GetIntElement(in, "Value")) {}

IEImpulse::IEImpulse(const nlohmann::ordered_json& in)
: IntElement(in), m_impulse(ParticleDataFactory::GetIntElement(in, "Value")) {}

void IEImpulse::PutToInternal(athena::io::IStreamWriter& out) const { m_impulse->PutTo(out); }

void IEImpulse::PutToInternal(nlohmann::ordered_json& out) const { m_impulse->PutTo(out); }

IELifetimePercent::IELifetimePercent(athena::io::IStreamReader& in)
: IntElement(in), m_lifetime(ParticleDataFactory::GetIntElement(in, "Value")) {}

IELifetimePercent::IELifetimePercent(const nlohmann::ordered_json& in)
: IntElement(in), m_lifetime(ParticleDataFactory::GetIntElement(in, "Value")) {}

void IELifetimePercent::PutToInternal(athena::io::IStreamWriter& out) const { m_lifetime->PutTo(out); }

void IELifetimePercent::PutToInternal(nlohmann::ordered_json& out) const { m_lifetime->PutTo(out); }

IESampleAndHold::IESampleAndHold(athena::io::IStreamReader& in)
: IntElement(in)
, m_sampleSource(ParticleDataFactory::GetIntElement(in, "SampleSource"))
, m_waitFramesMin(ParticleDataFactory::GetIntElement(in, "WaitFramesMin"))
, m_waitFramesMax(ParticleDataFactory::GetIntElement(in, "WaitFramesMax")) {}

IESampleAndHold::IESampleAndHold(const nlohmann::ordered_json& in)
: IntElement(in)
, m_sampleSource(ParticleDataFactory::GetIntElement(in, "SampleSource"))
, m_waitFramesMin(ParticleDataFactory::GetIntElement(in, "WaitFramesMin"))
, m_waitFramesMax(ParticleDataFactory::GetIntElement(in, "WaitFramesMax")) {}

void IESampleAndHold::PutToInternal(athena::io::IStreamWriter& out) const {
  if (!m_sampleSource || !m_waitFramesMin || m_waitFramesMax) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('SPAH'));
  m_sampleSource->PutTo(out);
  m_waitFramesMin->PutTo(out);
  m_waitFramesMax->PutTo(out);
}

void IESampleAndHold::PutToInternal(nlohmann::ordered_json& out) const {
  m_sampleSource->PutTo(out);
  m_waitFramesMin->PutTo(out);
  m_waitFramesMax->PutTo(out);
}

IEInitialRandom::IEInitialRandom(athena::io::IStreamReader& in)
: IntElement(in)
, m_min(ParticleDataFactory::GetIntElement(in, "Min"))
, m_max(ParticleDataFactory::GetIntElement(in, "Max")) {}

IEInitialRandom::IEInitialRandom(const nlohmann::ordered_json& in)
: IntElement(in)
, m_min(ParticleDataFactory::GetIntElement(in, "Min"))
, m_max(ParticleDataFactory::GetIntElement(in, "Max")) {}

void IEInitialRandom::PutToInternal(athena::io::IStreamWriter& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
}

void IEInitialRandom::PutToInternal(nlohmann::ordered_json& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
}

IEClamp::IEClamp(athena::io::IStreamReader& in)
: IntElement(in)
, m_min(ParticleDataFactory::GetIntElement(in, "Min"))
, m_max(ParticleDataFactory::GetIntElement(in, "Max"))
, m_value(ParticleDataFactory::GetIntElement(in, "Value")) {}

IEClamp::IEClamp(const nlohmann::ordered_json& in)
: IntElement(in)
, m_min(ParticleDataFactory::GetIntElement(in, "Min"))
, m_max(ParticleDataFactory::GetIntElement(in, "Max"))
, m_value(ParticleDataFactory::GetIntElement(in, "Value")) {}

void IEClamp::PutToInternal(athena::io::IStreamWriter& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
  m_value->PutTo(out);
}

void IEClamp::PutToInternal(nlohmann::ordered_json& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
  m_value->PutTo(out);
}

IEPulse::IEPulse(athena::io::IStreamReader& in)
: IntElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetIntElement(in, "A"))
, m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

IEPulse::IEPulse(const nlohmann::ordered_json& in)
: IntElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetIntElement(in, "A"))
, m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

IEPulse::~IEPulse() {}

void IEPulse::PutToInternal(athena::io::IStreamWriter& out) const {
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IEPulse::PutToInternal(nlohmann::ordered_json& out) const {
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

IERealToInt::IERealToInt(athena::io::IStreamReader& in)
: IntElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

IERealToInt::IERealToInt(const nlohmann::ordered_json& in)
: IntElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

IERealToInt::~IERealToInt() {}

void IERealToInt::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IERealToInt::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

IESubtract::IESubtract(athena::io::IStreamReader& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

IESubtract::IESubtract(const nlohmann::ordered_json& in)
: IntElement(in), m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetIntElement(in, "B")) {}

void IESubtract::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void IESubtract::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

} // namespace axdl::primedep::particles