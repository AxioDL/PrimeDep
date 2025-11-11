#include "PrimeDep/Particles/RealElement.hpp"

#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
REConstant::REConstant(athena::io::IStreamReader& in) : RealElement(in), m_value(ParticleDataFactory::GetReal(in)) {}
REConstant::REConstant(const nlohmann::ordered_json& in) : RealElement(in), m_value(in.value("Value", 0)) {}

void REConstant::PutToInternal(athena::io::IStreamWriter& out) const { ParticleDataFactory::SetReal(out, m_value); }

void REConstant::PutToInternal(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetReal(out, "Value", m_value);
}

REKeyframeEmitter::REKeyframeEmitter(athena::io::IStreamReader& in)
: RealElement(in)
, m_percent(in.readUint32Big())
, m_unknown1(in.readUint32Big())
, m_loop(in.readBool())
, m_unknown2(in.readBool())
, m_loopEnd(in.readInt32Big())
, m_loopStart(in.readInt32Big()) {
  int keyCount = in.readUint32Big();
  while (keyCount--) {
    m_keys.emplace_back(in.readFloatBig());
  }
}

REKeyframeEmitter::REKeyframeEmitter(const nlohmann::ordered_json& in)
: RealElement(in)
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

void REKeyframeEmitter::PutToInternal(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(m_percent);
  out.writeUint32Big(m_unknown1);
  out.writeBool(m_loop);
  out.writeBool(m_unknown2);
  out.writeInt32Big(m_loopEnd);
  out.writeInt32Big(m_loopStart);
  out.writeUint32Big(m_keys.size());
  for (const auto& key : m_keys) {
    out.writeFloatBig(key);
  }
}

void REKeyframeEmitter::PutToInternal(nlohmann::ordered_json& out) const {
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

RETimeScale::RETimeScale(athena::io::IStreamReader& in)
: RealElement(in), m_scale(ParticleDataFactory::GetRealElement(in, "Scale")) {}
RETimeScale::RETimeScale(const nlohmann::ordered_json& in)
: RealElement(in), m_scale(ParticleDataFactory::GetRealElement(in, "Scale")) {}

RETimeScale::~RETimeScale() {}

void RETimeScale::PutToInternal(athena::io::IStreamWriter& out) const { m_scale->PutTo(out); }

void RETimeScale::PutToInternal(nlohmann::ordered_json& out) const { m_scale->PutTo(out); }

RESineWave::RESineWave(athena::io::IStreamReader& in)
: RealElement(in)
, m_frequency(ParticleDataFactory::GetRealElement(in, "Frequency"))
, m_amplitude(ParticleDataFactory::GetRealElement(in, "Amplitude"))
, m_phase(ParticleDataFactory::GetRealElement(in, "Phase")) {}

RESineWave::RESineWave(const nlohmann::ordered_json& in)
: RealElement(in)
, m_frequency(ParticleDataFactory::GetRealElement(in, "Frequency"))
, m_amplitude(ParticleDataFactory::GetRealElement(in, "Amplitude"))
, m_phase(ParticleDataFactory::GetRealElement(in, "Phase")) {}

void RESineWave::PutToInternal(athena::io::IStreamWriter& out) const {
  if (!m_frequency || !m_amplitude) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('SINE'));
  m_frequency->PutTo(out);
  m_amplitude->PutTo(out);
}

void RESineWave::PutToInternal(nlohmann::ordered_json& out) const {
  m_frequency->PutTo(out);
  m_amplitude->PutTo(out);
  m_phase->PutTo(out);
}

REAdd::REAdd(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

REAdd::REAdd(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

void REAdd::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REAdd::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

REMultiply::REMultiply(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

REMultiply::REMultiply(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

void REMultiply::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REMultiply::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

REDotProduct::REDotProduct(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

REDotProduct::REDotProduct(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

REDotProduct::~REDotProduct() {}

void REDotProduct::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REDotProduct::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

RERandom::RERandom(athena::io::IStreamReader& in)
: RealElement(in)
, m_min(ParticleDataFactory::GetRealElement(in, "Min"))
, m_max(ParticleDataFactory::GetRealElement(in, "Max")) {}

RERandom::RERandom(const nlohmann::ordered_json& in)
: RealElement(in)
, m_min(ParticleDataFactory::GetRealElement(in, "Min"))
, m_max(ParticleDataFactory::GetRealElement(in, "Max")) {}

void RERandom::PutToInternal(athena::io::IStreamWriter& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
}

void RERandom::PutToInternal(nlohmann::ordered_json& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
}

REInitialRandom::REInitialRandom(athena::io::IStreamReader& in)
: RealElement(in)
, m_min(ParticleDataFactory::GetRealElement(in, "Min"))
, m_max(ParticleDataFactory::GetRealElement(in, "Max")) {}

REInitialRandom::REInitialRandom(const nlohmann::ordered_json& in)
: RealElement(in)
, m_min(ParticleDataFactory::GetRealElement(in, "Min"))
, m_max(ParticleDataFactory::GetRealElement(in, "Max")) {}

void REInitialRandom::PutToInternal(athena::io::IStreamWriter& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
}

void REInitialRandom::PutToInternal(nlohmann::ordered_json& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
}

RETimeChain::RETimeChain(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

RETimeChain::RETimeChain(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

RETimeChain::~RETimeChain() {}

void RETimeChain::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void RETimeChain::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

REClamp::REClamp(athena::io::IStreamReader& in)
: RealElement(in)
, m_min(ParticleDataFactory::GetRealElement(in, "Min"))
, m_max(ParticleDataFactory::GetRealElement(in, "Max"))
, m_value(ParticleDataFactory::GetRealElement(in, "Value")) {}

REClamp::REClamp(const nlohmann::ordered_json& in)
: RealElement(in)
, m_min(ParticleDataFactory::GetRealElement(in, "Min"))
, m_max(ParticleDataFactory::GetRealElement(in, "Max"))
, m_value(ParticleDataFactory::GetRealElement(in, "Value")) {}

void REClamp::PutToInternal(athena::io::IStreamWriter& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
  m_value->PutTo(out);
}

void REClamp::PutToInternal(nlohmann::ordered_json& out) const {
  m_min->PutTo(out);
  m_max->PutTo(out);
  m_value->PutTo(out);
}

REPulse::REPulse(athena::io::IStreamReader& in)
: RealElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

REPulse::REPulse(const nlohmann::ordered_json& in)
: RealElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

REPulse::~REPulse() {}

void REPulse::PutToInternal(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PULS'));
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REPulse::PutToInternal(nlohmann::ordered_json& out) const {
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

RELifetimePercent::RELifetimePercent(athena::io::IStreamReader& in)
: RealElement(in), m_lifetime(ParticleDataFactory::GetRealElement(in, "Value")) {}

RELifetimePercent::RELifetimePercent(const nlohmann::ordered_json& in)
: RealElement(in), m_lifetime(ParticleDataFactory::GetRealElement(in, "Value")) {}

void RELifetimePercent::PutToInternal(athena::io::IStreamWriter& out) const { m_lifetime->PutTo(out); }

void RELifetimePercent::PutToInternal(nlohmann::ordered_json& out) const { m_lifetime->PutTo(out); }

RELifetimeTween::RELifetimeTween(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

RELifetimeTween::RELifetimeTween(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

void RELifetimeTween::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void RELifetimeTween::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

REVectorXToReal::REVectorXToReal(athena::io::IStreamReader& in)
: RealElement(in), m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}
REVectorXToReal::REVectorXToReal(const nlohmann::ordered_json& in)
: RealElement(in), m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

REVectorXToReal::~REVectorXToReal() {}

void REVectorXToReal::PutToInternal(athena::io::IStreamWriter& out) const { m_vector->PutTo(out); }

void REVectorXToReal::PutToInternal(nlohmann::ordered_json& out) const { m_vector->PutTo(out); }

REVectorYToReal::REVectorYToReal(athena::io::IStreamReader& in)
: RealElement(in), m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}
REVectorYToReal::REVectorYToReal(const nlohmann::ordered_json& in)
: RealElement(in), m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

REVectorYToReal::~REVectorYToReal() {}

void REVectorYToReal::PutToInternal(athena::io::IStreamWriter& out) const { m_vector->PutTo(out); }

void REVectorYToReal::PutToInternal(nlohmann::ordered_json& out) const { m_vector->PutTo(out); }

REVectorZToReal::REVectorZToReal(athena::io::IStreamReader& in)
: RealElement(in), m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}
REVectorZToReal::REVectorZToReal(const nlohmann::ordered_json& in)
: RealElement(in), m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

REVectorZToReal::~REVectorZToReal() {}

void REVectorZToReal::PutToInternal(athena::io::IStreamWriter& out) const { m_vector->PutTo(out); }

void REVectorZToReal::PutToInternal(nlohmann::ordered_json& out) const { m_vector->PutTo(out); }

REVectorMagnitude::REVectorMagnitude(athena::io::IStreamReader& in)
: RealElement(in), m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}
REVectorMagnitude::REVectorMagnitude(const nlohmann::ordered_json& in)
: RealElement(in), m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

REVectorMagnitude::~REVectorMagnitude() {}

void REVectorMagnitude::PutToInternal(athena::io::IStreamWriter& out) const { m_vector->PutTo(out); }

void REVectorMagnitude::PutToInternal(nlohmann::ordered_json& out) const { m_vector->PutTo(out); }

RECompareLessThan::RECompareLessThan(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D")) {}

RECompareLessThan::RECompareLessThan(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D")) {}

void RECompareLessThan::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
}

void RECompareLessThan::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
}

RECompareEqual::RECompareEqual(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D")) {}

RECompareEqual::RECompareEqual(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D")) {}

void RECompareEqual::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
}

void RECompareEqual::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
}

REConstantRange::REConstantRange(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D"))
, m_e(ParticleDataFactory::GetRealElement(in, "E")) {}

REConstantRange::REConstantRange(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D"))
, m_e(ParticleDataFactory::GetRealElement(in, "E")) {}

void REConstantRange::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
  m_e->PutTo(out);
}

void REConstantRange::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
  m_e->PutTo(out);
}

REExternalVar::REExternalVar(athena::io::IStreamReader& in)
: RealElement(in), m_value(ParticleDataFactory::GetIntElement(in, "Value")) {}

REExternalVar::REExternalVar(const nlohmann::ordered_json& in)
: RealElement(in), m_value(ParticleDataFactory::GetIntElement(in, "Value")) {}

REExternalVar::~REExternalVar() {}

void REExternalVar::PutToInternal(athena::io::IStreamWriter& out) const { m_value->PutTo(out); }

void REExternalVar::PutToInternal(nlohmann::ordered_json& out) const { m_value->PutTo(out); }

REIntTimesReal::REIntTimesReal(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetIntElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}
REIntTimesReal::REIntTimesReal(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetIntElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

REIntTimesReal::~REIntTimesReal() {}

void REIntTimesReal::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REIntTimesReal::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

RESubtract::RESubtract(athena::io::IStreamReader& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

RESubtract::RESubtract(const nlohmann::ordered_json& in)
: RealElement(in)
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

RESubtract::~RESubtract() {}

void RESubtract::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void RESubtract::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

REGetComponentRed::REGetComponentRed(athena::io::IStreamReader& in)
: RealElement(in), m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentRed::REGetComponentRed(const nlohmann::ordered_json& in)
: RealElement(in), m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentRed::~REGetComponentRed() {}

void REGetComponentRed::PutToInternal(athena::io::IStreamWriter& out) const { m_color->PutTo(out); }

void REGetComponentRed::PutToInternal(nlohmann::ordered_json& out) const { m_color->PutTo(out); }

REGetComponentGreen::REGetComponentGreen(athena::io::IStreamReader& in)
: RealElement(in), m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentGreen::REGetComponentGreen(const nlohmann::ordered_json& in)
: RealElement(in), m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentGreen::~REGetComponentGreen() {}

void REGetComponentGreen::PutToInternal(athena::io::IStreamWriter& out) const { m_color->PutTo(out); }

void REGetComponentGreen::PutToInternal(nlohmann::ordered_json& out) const { m_color->PutTo(out); }

REGetComponentBlue::REGetComponentBlue(athena::io::IStreamReader& in)
: RealElement(in), m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentBlue::REGetComponentBlue(const nlohmann::ordered_json& in)
: RealElement(in), m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentBlue::~REGetComponentBlue() {}

void REGetComponentBlue::PutToInternal(athena::io::IStreamWriter& out) const { m_color->PutTo(out); }

void REGetComponentBlue::PutToInternal(nlohmann::ordered_json& out) const { m_color->PutTo(out); }

REGetComponentAlpha::REGetComponentAlpha(athena::io::IStreamReader& in)
: RealElement(in), m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentAlpha::REGetComponentAlpha(const nlohmann::ordered_json& in)
: RealElement(in), m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentAlpha::~REGetComponentAlpha() {}

void REGetComponentAlpha::PutToInternal(athena::io::IStreamWriter& out) const { m_color->PutTo(out); }

void REGetComponentAlpha::PutToInternal(nlohmann::ordered_json& out) const { m_color->PutTo(out); }

} // namespace axdl::primedep::particles