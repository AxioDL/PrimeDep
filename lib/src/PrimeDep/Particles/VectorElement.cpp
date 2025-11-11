#include "PrimeDep/Particles/VectorElement.hpp"

#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ParticleData.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/RealElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
VEConstant::VEConstant(athena::io::IStreamReader& in)
: VectorElement(in)
, m_x(ParticleDataFactory::GetRealElement(in, "X"))
, m_y(ParticleDataFactory::GetRealElement(in, "Y"))
, m_z(ParticleDataFactory::GetRealElement(in, "Z")) {}

VEConstant::VEConstant(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_x(ParticleDataFactory::GetRealElement(in, "X"))
, m_y(ParticleDataFactory::GetRealElement(in, "Y"))
, m_z(ParticleDataFactory::GetRealElement(in, "Z")) {}

VEConstant::~VEConstant() {}

void VEConstant::PutToInternal(athena::io::IStreamWriter& out) const {
  m_x->PutTo(out);
  m_y->PutTo(out);
  m_z->PutTo(out);
}

void VEConstant::PutToInternal(nlohmann::ordered_json& out) const {
  m_x->PutTo(out);
  m_y->PutTo(out);
  m_z->PutTo(out);
}

VEKeyframeEmitter::VEKeyframeEmitter(athena::io::IStreamReader& in)
: VectorElement(in)
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

VEKeyframeEmitter::VEKeyframeEmitter(const nlohmann::ordered_json& in)
: VectorElement(in)
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

void VEKeyframeEmitter::PutToInternal(athena::io::IStreamWriter& out) const {
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

void VEKeyframeEmitter::PutToInternal(nlohmann::ordered_json& out) const {
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
: VectorElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

VETimeChain::VETimeChain(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

VETimeChain::~VETimeChain() {}

void VETimeChain::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void VETimeChain::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

VEAngleCone::VEAngleCone(athena::io::IStreamReader& in)
: VectorElement(in)
, m_angleConstantX(ParticleDataFactory::GetRealElement(in, "AngleConstantX"sv))
, m_angleConstantY(ParticleDataFactory::GetRealElement(in, "AngleConstantY"sv))
, m_angleXRange(ParticleDataFactory::GetRealElement(in, "AngleXRange"sv))
, m_angleYRange(ParticleDataFactory::GetRealElement(in, "AngleYRange"sv))
, m_magnitude(ParticleDataFactory::GetRealElement(in, "Magnitude"sv)) {}

VEAngleCone::VEAngleCone(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_angleConstantX(ParticleDataFactory::GetRealElement(in, "AngleConstantX"sv))
, m_angleConstantY(ParticleDataFactory::GetRealElement(in, "AngleConstantY"sv))
, m_angleXRange(ParticleDataFactory::GetRealElement(in, "AngleXRange"sv))
, m_angleYRange(ParticleDataFactory::GetRealElement(in, "AngleYRange"sv))
, m_magnitude(ParticleDataFactory::GetRealElement(in, "Magnitude"sv)) {}

VEAngleCone::~VEAngleCone() {}

void VEAngleCone::PutToInternal(athena::io::IStreamWriter& out) const {
  m_angleConstantX->PutTo(out);
  m_angleConstantY->PutTo(out);
  m_angleXRange->PutTo(out);
  m_angleYRange->PutTo(out);
  m_magnitude->PutTo(out);
}

void VEAngleCone::PutToInternal(nlohmann::ordered_json& out) const {
  m_angleConstantX->PutTo(out);
  m_angleConstantY->PutTo(out);
  m_angleXRange->PutTo(out);
  m_angleYRange->PutTo(out);
  m_magnitude->PutTo(out);
}

VEAdd::VEAdd(athena::io::IStreamReader& in)
: VectorElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "A")) {}

VEAdd::VEAdd(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "A")) {}

void VEAdd::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void VEAdd::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

VECircleCluster::VECircleCluster(athena::io::IStreamReader& in)
: VectorElement(in)
, m_direction(ParticleDataFactory::GetVectorElement(in, "Direction"))
, m_upVector(ParticleDataFactory::GetVectorElement(in, "UpVector"))
, m_deltaAngle(ParticleDataFactory::GetIntElement(in, "DeltaAngle"))
, m_magnitude(ParticleDataFactory::GetRealElement(in, "Magnitude")) {}
VECircleCluster::VECircleCluster(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_direction(ParticleDataFactory::GetVectorElement(in, "Direction"))
, m_upVector(ParticleDataFactory::GetVectorElement(in, "UpVector"))
, m_deltaAngle(ParticleDataFactory::GetIntElement(in, "DeltaAngle"))
, m_magnitude(ParticleDataFactory::GetRealElement(in, "Magnitude")) {}

VECircleCluster::~VECircleCluster() {}

void VECircleCluster::PutToInternal(athena::io::IStreamWriter& out) const {
  m_direction->PutTo(out);
  m_upVector->PutTo(out);
  m_deltaAngle->PutTo(out);
  m_magnitude->PutTo(out);
}

void VECircleCluster::PutToInternal(nlohmann::ordered_json& out) const {
  m_direction->PutTo(out);
  m_upVector->PutTo(out);
  m_deltaAngle->PutTo(out);
  m_magnitude->PutTo(out);
}

VECircle::VECircle(athena::io::IStreamReader& in)
: VectorElement(in)
, m_direction(ParticleDataFactory::GetVectorElement(in, "Direction"))
, m_upVector(ParticleDataFactory::GetVectorElement(in, "UpVector"))
, m_angleConstant(ParticleDataFactory::GetRealElement(in, "AngleConstant"))
, m_angleLinear(ParticleDataFactory::GetRealElement(in, "AngleLinear"))
, m_radius(ParticleDataFactory::GetRealElement(in, "Radius")) {}

VECircle::VECircle(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_direction(ParticleDataFactory::GetVectorElement(in, "Direction"))
, m_upVector(ParticleDataFactory::GetVectorElement(in, "UpVector"))
, m_angleConstant(ParticleDataFactory::GetRealElement(in, "AngleConstant"))
, m_angleLinear(ParticleDataFactory::GetRealElement(in, "AngleLinear"))
, m_radius(ParticleDataFactory::GetRealElement(in, "Radius")) {}

VECircle::~VECircle() {}

void VECircle::PutToInternal(athena::io::IStreamWriter& out) const {
  m_direction->PutTo(out);
  m_upVector->PutTo(out);
  m_angleConstant->PutTo(out);
  m_angleLinear->PutTo(out);
  m_radius->PutTo(out);
}

void VECircle::PutToInternal(nlohmann::ordered_json& out) const {
  m_direction->PutTo(out);
  m_upVector->PutTo(out);
  m_angleConstant->PutTo(out);
  m_angleLinear->PutTo(out);
  m_radius->PutTo(out);
}

VEMultiply::VEMultiply(athena::io::IStreamReader& in)
: VectorElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

VEMultiply::VEMultiply(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

void VEMultiply::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void VEMultiply::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

VERealToVector::VERealToVector(athena::io::IStreamReader& in)
: VectorElement(in), m_value(ParticleDataFactory::GetRealElement(in, "Value")) {}

VERealToVector::VERealToVector(const nlohmann::ordered_json& in)
: VectorElement(in), m_value(ParticleDataFactory::GetRealElement(in, "Value")) {}

VERealToVector::~VERealToVector() {}

void VERealToVector::PutToInternal(athena::io::IStreamWriter& out) const { m_value->PutTo(out); }

void VERealToVector::PutToInternal(nlohmann::ordered_json& out) const { m_value->PutTo(out); }

VEPulse::VEPulse(athena::io::IStreamReader& in)
: VectorElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

VEPulse::VEPulse(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

VEPulse::~VEPulse() {}

void VEPulse::PutToInternal(athena::io::IStreamWriter& out) const {
  m_aDuration->PutTo(out);
  m_bDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void VEPulse::PutToInternal(nlohmann::ordered_json& out) const {
  m_aDuration->PutTo(out);
  m_bDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

VESubtract::VESubtract(athena::io::IStreamReader& in)
: VectorElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

VESubtract::VESubtract(const nlohmann::ordered_json& in)
: VectorElement(in)
, m_a(ParticleDataFactory::GetVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetVectorElement(in, "B")) {}

void VESubtract::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void VESubtract::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
}

VEColorToVector::VEColorToVector(athena::io::IStreamReader& in)
: VectorElement(in), m_value(ParticleDataFactory::GetColorElement(in, "Value")) {}

VEColorToVector::VEColorToVector(const nlohmann::ordered_json& in)
: VectorElement(in), m_value(ParticleDataFactory::GetColorElement(in, "Value")) {}

VEColorToVector::~VEColorToVector() {}

void VEColorToVector::PutToInternal(athena::io::IStreamWriter& out) const { m_value->PutTo(out); }

void VEColorToVector::PutToInternal(nlohmann::ordered_json& out) const { m_value->PutTo(out); }

} // namespace axdl::primedep::particles