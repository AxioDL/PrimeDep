#include "PrimeDep/Particles/RealElement.hpp"

#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
void RENone::PutTo(athena::io::IStreamWriter& out) const { ParticleDataFactory::SetClassID(out, FOURCC('NONE')); }
void RENone::PutTo(nlohmann::ordered_json& out) const { ParticleDataFactory::SetClassID(out, "None"); }

REConstant::REConstant(athena::io::IStreamReader& in) : m_value(ParticleDataFactory::GetReal(in)) {}
REConstant::REConstant(const nlohmann::ordered_json& in) : m_value(in.value("Value", 0)) {}

void REConstant::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('CNST'));
  ParticleDataFactory::SetReal(out, m_value);
}

void REConstant::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "Constant");
  ParticleDataFactory::SetReal(out, "Value", m_value);
}

REKeyframeEmitter::REKeyframeEmitter(athena::io::IStreamReader& in)
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

REKeyframeEmitter::REKeyframeEmitter(const nlohmann::ordered_json& in)
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

void REKeyframeEmitter::PutTo(athena::io::IStreamWriter& out) const {
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

void REKeyframeEmitter::PutTo(nlohmann::ordered_json& out) const {
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

RETimeScale::RETimeScale(athena::io::IStreamReader& in) : m_scale(ParticleDataFactory::GetRealElement(in)) {}
RETimeScale::RETimeScale(const nlohmann::ordered_json& in)
: m_scale(ParticleDataFactory::GetRealElement(in, "Scale")) {}

RETimeScale::~RETimeScale() {}

void RETimeScale::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_scale) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('SCAL'));
  m_scale->PutTo(out);
}

void RETimeScale::PutTo(nlohmann::ordered_json& out) const {
  if (!m_scale) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "TimeScale"sv);
  m_scale->PutTo(out["Scale"]);
}

RESineWave::RESineWave(athena::io::IStreamReader& in)
: m_frequency(ParticleDataFactory::GetRealElement(in))
, m_amplitude(ParticleDataFactory::GetRealElement(in))
, m_phase(ParticleDataFactory::GetRealElement(in)) {}

RESineWave::RESineWave(const nlohmann::ordered_json& in)
: m_frequency(ParticleDataFactory::GetRealElement(in, "Frequency"))
, m_amplitude(ParticleDataFactory::GetRealElement(in, "Amplitude"))
, m_phase(ParticleDataFactory::GetRealElement(in, "Phase")) {}

void RESineWave::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_frequency || !m_amplitude) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('SINE'));
  m_frequency->PutTo(out);
  m_amplitude->PutTo(out);
}

void RESineWave::PutTo(nlohmann::ordered_json& out) const {
  if (!m_frequency || !m_amplitude) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "SineWave");
  m_frequency->PutTo(out["Frequency"]);
  m_amplitude->PutTo(out["Amplitude"]);
  m_phase->PutTo(out["Phase"]);
}

REAdd::REAdd(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in)), m_b(ParticleDataFactory::GetRealElement(in)) {}

REAdd::REAdd(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A")), m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

void REAdd::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('ADD_'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REAdd::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Add");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

REMultiply::REMultiply(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in)), m_b(ParticleDataFactory::GetRealElement(in)) {}

REMultiply::REMultiply(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A")), m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

void REMultiply::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('MULT'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REMultiply::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Multiply");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

REDotProduct::REDotProduct(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetVectorElement(in)), m_b(ParticleDataFactory::GetVectorElement(in)) {}

REDotProduct::REDotProduct(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetVectorElement(in, "A")), m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

REDotProduct::~REDotProduct() {}

void REDotProduct::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('DOTP'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REDotProduct::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "DotProduct");
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

RERandom::RERandom(athena::io::IStreamReader& in)
: m_min(ParticleDataFactory::GetRealElement(in)), m_max(ParticleDataFactory::GetRealElement(in)) {}

RERandom::RERandom(const nlohmann::ordered_json& in)
: m_min(ParticleDataFactory::GetRealElement(in, "Min")), m_max(ParticleDataFactory::GetRealElement(in, "Max")) {}

void RERandom::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_min || !m_max) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('RAND'));
  m_min->PutTo(out);
  m_max->PutTo(out);
}

void RERandom::PutTo(nlohmann::ordered_json& out) const {
  if (!m_min || !m_max) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "Random");
  m_min->PutTo(out["Min"]);
  m_max->PutTo(out["Max"]);
}

REInitialRandom::REInitialRandom(athena::io::IStreamReader& in)
: m_min(ParticleDataFactory::GetRealElement(in)), m_max(ParticleDataFactory::GetRealElement(in)) {}

REInitialRandom::REInitialRandom(const nlohmann::ordered_json& in)
: m_min(ParticleDataFactory::GetRealElement(in, "Min")), m_max(ParticleDataFactory::GetRealElement(in, "Max")) {}

void REInitialRandom::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_min || !m_max) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('IRND'));
  m_min->PutTo(out);
  m_max->PutTo(out);
}

void REInitialRandom::PutTo(nlohmann::ordered_json& out) const {
  if (!m_min || !m_max) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "InitialRandom");
  m_min->PutTo(out["Min"]);
  m_max->PutTo(out["Max"]);
}

RETimeChain::RETimeChain(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in))
, m_b(ParticleDataFactory::GetRealElement(in))
, m_switchFrame(ParticleDataFactory::GetIntElement(in)) {}

RETimeChain::RETimeChain(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

RETimeChain::~RETimeChain() {}

void RETimeChain::PutTo(athena::io::IStreamWriter& out) const {
  // Int Elements must have all 3 elements specified
  if (!m_a || !m_b || !m_switchFrame) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('CHAN'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void RETimeChain::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_switchFrame) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "TimeChain"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_switchFrame->PutTo(out["SwitchFrame"]);
}

REClamp::REClamp(athena::io::IStreamReader& in)
: m_min(ParticleDataFactory::GetRealElement(in))
, m_max(ParticleDataFactory::GetRealElement(in))
, m_value(ParticleDataFactory::GetRealElement(in)) {}

REClamp::REClamp(const nlohmann::ordered_json& in)
: m_min(ParticleDataFactory::GetRealElement(in, "Min"))
, m_max(ParticleDataFactory::GetRealElement(in, "Max"))
, m_value(ParticleDataFactory::GetRealElement(in, "Value")) {}

void REClamp::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_min || !m_max || m_value) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('CLMP'));
  m_min->PutTo(out);
  m_max->PutTo(out);
  m_value->PutTo(out);
}

void REClamp::PutTo(nlohmann::ordered_json& out) const {
  if (!m_min || !m_max || m_value) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Clamp"sv);
  m_min->PutTo(out["Min"]);
  m_max->PutTo(out["Max"]);
  m_value->PutTo(out["Value"]);
}

REPulse::REPulse(athena::io::IStreamReader& in)
: m_aDuration(ParticleDataFactory::GetIntElement(in))
, m_bDuration(ParticleDataFactory::GetIntElement(in))
, m_a(ParticleDataFactory::GetRealElement(in))
, m_b(ParticleDataFactory::GetRealElement(in)) {}

REPulse::REPulse(const nlohmann::ordered_json& in)
: m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

REPulse::~REPulse() {}

void REPulse::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_aDuration || !m_bDuration || !m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('PULS'));
  m_aDuration->PutTo(out);
  m_aDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REPulse::PutTo(nlohmann::ordered_json& out) const {
  if (!m_aDuration || !m_bDuration || !m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Pulse"sv);
  m_aDuration->PutTo(out["DurationA"]);
  m_aDuration->PutTo(out["DurationB"]);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

RELifetimePercent::RELifetimePercent(athena::io::IStreamReader& in)
: m_lifetime(ParticleDataFactory::GetRealElement(in)) {}

RELifetimePercent::RELifetimePercent(const nlohmann::ordered_json& in)
: m_lifetime(ParticleDataFactory::GetRealElement(in, "Value")) {}

void RELifetimePercent::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_lifetime) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('RLPT'));
  m_lifetime->PutTo(out);
}

void RELifetimePercent::PutTo(nlohmann::ordered_json& out) const {
  if (!m_lifetime) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "LifetimePercent");
  m_lifetime->PutTo(out["Value"]);
}

RELifetimeTween::RELifetimeTween(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in)), m_b(ParticleDataFactory::GetRealElement(in)) {}

RELifetimeTween::RELifetimeTween(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A")), m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

void RELifetimeTween::PutTo(athena::io::IStreamWriter& out) const {
  // Int Elements must have all 3 elements specified
  if (!m_a || !m_b) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('LFTW'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void RELifetimeTween::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "LifetimeTween"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

void REParticleRotationOrLineWidth::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PRLW'));
}
void REParticleRotationOrLineWidth::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleRotationOrLineWidth");
}

void REParticleSizeOrLineLength::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PSLL'));
}
void REParticleSizeOrLineLength::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleSizeOrLineLength");
}

void REParticleAccessParameter1::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PAP1'));
}
void REParticleAccessParameter1::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleAccessParameter1");
}

void REParticleAccessParameter2::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PAP2'));
}
void REParticleAccessParameter2::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleAccessParameter2");
}

void REParticleAccessParameter3::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PAP3'));
}
void REParticleAccessParameter3::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleAccessParameter3");
}

void REParticleAccessParameter4::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PAP4'));
}
void REParticleAccessParameter4::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleAccessParameter4");
}

void REParticleAccessParameter5::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PAP5'));
}
void REParticleAccessParameter5::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleAccessParameter5");
}

void REParticleAccessParameter6::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PAP6'));
}
void REParticleAccessParameter6::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleAccessParameter6");
}

void REParticleAccessParameter7::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PAP7'));
}
void REParticleAccessParameter7::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleAccessParameter7");
}

void REParticleAccessParameter8::PutTo(athena::io::IStreamWriter& out) const {
  ParticleDataFactory::SetClassID(out, FOURCC('PAP8'));
}
void REParticleAccessParameter8::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "ParticleAccessParameter8");
}

REVectorXToReal::REVectorXToReal(athena::io::IStreamReader& in) : m_vector(ParticleDataFactory::GetVectorElement(in)) {}
REVectorXToReal::REVectorXToReal(const nlohmann::ordered_json& in)
: m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

REVectorXToReal::~REVectorXToReal() {}

void REVectorXToReal::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_vector) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('VXTR'));
  m_vector->PutTo(out);
}

void REVectorXToReal::PutTo(nlohmann::ordered_json& out) const {
  if (!m_vector) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "VectorXToReal"sv);
  m_vector->PutTo(out["Vector"]);
}

REVectorYToReal::REVectorYToReal(athena::io::IStreamReader& in) : m_vector(ParticleDataFactory::GetVectorElement(in)) {}
REVectorYToReal::REVectorYToReal(const nlohmann::ordered_json& in)
: m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

REVectorYToReal::~REVectorYToReal() {}

void REVectorYToReal::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_vector) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('VYTR'));
  m_vector->PutTo(out);
}

void REVectorYToReal::PutTo(nlohmann::ordered_json& out) const {
  if (!m_vector) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "VectorYToReal"sv);
  m_vector->PutTo(out["Vector"]);
}

REVectorZToReal::REVectorZToReal(athena::io::IStreamReader& in) : m_vector(ParticleDataFactory::GetVectorElement(in)) {}
REVectorZToReal::REVectorZToReal(const nlohmann::ordered_json& in)
: m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

REVectorZToReal::~REVectorZToReal() {}

void REVectorZToReal::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_vector) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('VZTR'));
  m_vector->PutTo(out);
}

void REVectorZToReal::PutTo(nlohmann::ordered_json& out) const {
  if (!m_vector) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "VectorZToReal"sv);
  m_vector->PutTo(out["Vector"]);
}

REVectorMagnitude::REVectorMagnitude(athena::io::IStreamReader& in)
: m_vector(ParticleDataFactory::GetVectorElement(in)) {}
REVectorMagnitude::REVectorMagnitude(const nlohmann::ordered_json& in)
: m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

REVectorMagnitude::~REVectorMagnitude() {}

void REVectorMagnitude::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_vector) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('VMAG'));
  m_vector->PutTo(out);
}

void REVectorMagnitude::PutTo(nlohmann::ordered_json& out) const {
  if (!m_vector) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "VectorMagnitude"sv);
  m_vector->PutTo(out["Vector"]);
}

RECompareLessThan::RECompareLessThan(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in))
, m_b(ParticleDataFactory::GetRealElement(in))
, m_c(ParticleDataFactory::GetRealElement(in))
, m_d(ParticleDataFactory::GetRealElement(in)) {}

RECompareLessThan::RECompareLessThan(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D")) {}

void RECompareLessThan::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b || !m_c || !m_d) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('CLTN'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
}

void RECompareLessThan::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_c || !m_d) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "CompareLessThan"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_c->PutTo(out["C"]);
  m_d->PutTo(out["D"]);
}

RECompareEqual::RECompareEqual(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in))
, m_b(ParticleDataFactory::GetRealElement(in))
, m_c(ParticleDataFactory::GetRealElement(in))
, m_d(ParticleDataFactory::GetRealElement(in)) {}

RECompareEqual::RECompareEqual(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D")) {}

void RECompareEqual::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b || !m_c || !m_d) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('CEQL'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
}

void RECompareEqual::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_c || !m_d) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "CompareEqual"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_c->PutTo(out["C"]);
  m_d->PutTo(out["D"]);
}

REConstantRange::REConstantRange(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in))
, m_b(ParticleDataFactory::GetRealElement(in))
, m_c(ParticleDataFactory::GetRealElement(in))
, m_d(ParticleDataFactory::GetRealElement(in))
, m_e(ParticleDataFactory::GetRealElement(in)) {}

REConstantRange::REConstantRange(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A"))
, m_b(ParticleDataFactory::GetRealElement(in, "B"))
, m_c(ParticleDataFactory::GetRealElement(in, "C"))
, m_d(ParticleDataFactory::GetRealElement(in, "D"))
, m_e(ParticleDataFactory::GetRealElement(in, "E")) {}

void REConstantRange::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b || !m_c || !m_d || !m_e) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('CRNG'));
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_c->PutTo(out);
  m_d->PutTo(out);
  m_e->PutTo(out);
}

void REConstantRange::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b || !m_c || !m_d || !m_e) {
    return;
  }

  ParticleDataFactory::SetClassID(out, "ConstantRange"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
  m_c->PutTo(out["C"]);
  m_d->PutTo(out["D"]);
  m_e->PutTo(out["D"]);
  m_e->PutTo(out["E"]);
}

REExternalVar::REExternalVar(athena::io::IStreamReader& in) : m_value(ParticleDataFactory::GetIntElement(in)) {}

REExternalVar::REExternalVar(const nlohmann::ordered_json& in)
: m_value(ParticleDataFactory::GetIntElement(in, "Value")) {}

REExternalVar::~REExternalVar() {}

void REExternalVar::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_value) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('CEXT'));
  m_value->PutTo(out);
}

void REExternalVar::PutTo(nlohmann::ordered_json& out) const {
  if (!m_value) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "ExternalVar"sv);
  m_value->PutTo(out["Value"]);
}

REIntTimesReal::REIntTimesReal(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetIntElement(in)), m_b(ParticleDataFactory::GetRealElement(in)) {}

REIntTimesReal::REIntTimesReal(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetIntElement(in, "A")), m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

REIntTimesReal::~REIntTimesReal() {}

void REIntTimesReal::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('ITRL'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void REIntTimesReal::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "IntTimesReal"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

RESubtract::RESubtract(athena::io::IStreamReader& in)
: m_a(ParticleDataFactory::GetRealElement(in)), m_b(ParticleDataFactory::GetRealElement(in)) {}

RESubtract::RESubtract(const nlohmann::ordered_json& in)
: m_a(ParticleDataFactory::GetRealElement(in, "A")), m_b(ParticleDataFactory::GetRealElement(in, "B")) {}

RESubtract::~RESubtract() {}

void RESubtract::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('SUB_'));
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void RESubtract::PutTo(nlohmann::ordered_json& out) const {
  if (!m_a || !m_b) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "Subtract"sv);
  m_a->PutTo(out["A"]);
  m_b->PutTo(out["B"]);
}

REGetComponentRed::REGetComponentRed(athena::io::IStreamReader& in)
: m_color(ParticleDataFactory::GetColorElement(in)) {}

REGetComponentRed::REGetComponentRed(const nlohmann::ordered_json& in)
: m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentRed::~REGetComponentRed() {}

void REGetComponentRed::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_color) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('GTCR'));
  m_color->PutTo(out);
}

void REGetComponentRed::PutTo(nlohmann::ordered_json& out) const {
  if (!m_color) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "GetComponentRed"sv);
  m_color->PutTo(out["Color"]);
}

REGetComponentGreen::REGetComponentGreen(athena::io::IStreamReader& in)
: m_color(ParticleDataFactory::GetColorElement(in)) {}

REGetComponentGreen::REGetComponentGreen(const nlohmann::ordered_json& in)
: m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentGreen::~REGetComponentGreen() {}

void REGetComponentGreen::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_color) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('GTCG'));
  m_color->PutTo(out);
}

void REGetComponentGreen::PutTo(nlohmann::ordered_json& out) const {
  if (!m_color) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "GetComponentGreen"sv);
  m_color->PutTo(out["Color"]);
}

REGetComponentBlue::REGetComponentBlue(athena::io::IStreamReader& in)
: m_color(ParticleDataFactory::GetColorElement(in)) {}

REGetComponentBlue::REGetComponentBlue(const nlohmann::ordered_json& in)
: m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentBlue::~REGetComponentBlue() {}

void REGetComponentBlue::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_color) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('GTCG'));
  m_color->PutTo(out);
}

void REGetComponentBlue::PutTo(nlohmann::ordered_json& out) const {
  if (!m_color) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "GetComponentBlue"sv);
  m_color->PutTo(out["Color"]);
}

REGetComponentAlpha::REGetComponentAlpha(athena::io::IStreamReader& in)
: m_color(ParticleDataFactory::GetColorElement(in)) {}

REGetComponentAlpha::REGetComponentAlpha(const nlohmann::ordered_json& in)
: m_color(ParticleDataFactory::GetColorElement(in, "Color")) {}

REGetComponentAlpha::~REGetComponentAlpha() {}

void REGetComponentAlpha::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_color) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('GTCG'));
  m_color->PutTo(out);
}

void REGetComponentAlpha::PutTo(nlohmann::ordered_json& out) const {
  if (!m_color) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "GetComponentAlpha"sv);
  m_color->PutTo(out["Color"]);
}

} // namespace axdl::primedep::particles