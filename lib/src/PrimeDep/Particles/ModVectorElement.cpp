#include "PrimeDep/Particles/ModVectorElement.hpp"

#include "PrimeDep/FourCC.hpp"
#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ParticleData.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
MVEConstant::MVEConstant(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_x(ParticleDataFactory::GetRealElement(in, "X"))
, m_y(ParticleDataFactory::GetRealElement(in, "Y"))
, m_z(ParticleDataFactory::GetRealElement(in, "Z")) {}

MVEConstant::MVEConstant(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_x(ParticleDataFactory::GetRealElement(in, "X"))
, m_y(ParticleDataFactory::GetRealElement(in, "Y"))
, m_z(ParticleDataFactory::GetRealElement(in, "Z")) {}

MVEConstant::~MVEConstant() {}

void MVEConstant::PutToInternal(athena::io::IStreamWriter& out) const {
  m_x->PutTo(out);
  m_y->PutTo(out);
  m_z->PutTo(out);
}

void MVEConstant::PutToInternal(nlohmann::ordered_json& out) const {
  m_x->PutTo(out);
  m_y->PutTo(out);
  m_z->PutTo(out);
}

MVEGravity::MVEGravity(athena::io::IStreamReader& in)
: ModVectorElement(in), m_acceleration(ParticleDataFactory::GetVectorElement(in, "Acceleration")) {}
MVEGravity::MVEGravity(const nlohmann::ordered_json& in)
: ModVectorElement(in), m_acceleration(ParticleDataFactory::GetVectorElement(in, "Acceleration")) {}

MVEGravity::~MVEGravity() {}

void MVEGravity::PutToInternal(athena::io::IStreamWriter& out) const { m_acceleration->PutTo(out); }

void MVEGravity::PutToInternal(nlohmann::ordered_json& out) const { m_acceleration->PutTo(out); }

MVEWind::MVEWind(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_velocity(ParticleDataFactory::GetVectorElement(in, "Velocity"))
, m_factor(ParticleDataFactory::GetRealElement(in, "Factor")) {}
MVEWind::MVEWind(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_velocity(ParticleDataFactory::GetVectorElement(in, "Velocity"))
, m_factor(ParticleDataFactory::GetRealElement(in, "Factor")) {}

MVEWind::~MVEWind() {}

void MVEWind::PutToInternal(athena::io::IStreamWriter& out) const {
  m_velocity->PutTo(out);
  m_factor->PutTo(out);
}

void MVEWind::PutToInternal(nlohmann::ordered_json& out) const {
  m_velocity->PutTo(out);
  m_factor->PutTo(out);
}

MVEExplode::MVEExplode(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_impulseMagnitude(ParticleDataFactory::GetRealElement(in, "ImpulseMagnitude"))
, m_fallOffFactor(ParticleDataFactory::GetRealElement(in, "FallOffFactor")) {}
MVEExplode::MVEExplode(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_impulseMagnitude(ParticleDataFactory::GetRealElement(in, "ImpulseMagnitude"))
, m_fallOffFactor(ParticleDataFactory::GetRealElement(in, "FallOffFactor")) {}

MVEExplode::~MVEExplode() {}

void MVEExplode::PutToInternal(athena::io::IStreamWriter& out) const {
  m_impulseMagnitude->PutTo(out);
  m_fallOffFactor->PutTo(out);
}

void MVEExplode::PutToInternal(nlohmann::ordered_json& out) const {
  m_impulseMagnitude->PutTo(out);
  m_fallOffFactor->PutTo(out);
}

MVETimeChain::MVETimeChain(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_a(ParticleDataFactory::GetModVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetModVectorElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

MVETimeChain::MVETimeChain(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_a(ParticleDataFactory::GetModVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetModVectorElement(in, "B"))
, m_switchFrame(ParticleDataFactory::GetIntElement(in, "SwitchFrame")) {}

MVETimeChain::~MVETimeChain() {}

void MVETimeChain::PutToInternal(athena::io::IStreamWriter& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

void MVETimeChain::PutToInternal(nlohmann::ordered_json& out) const {
  m_a->PutTo(out);
  m_b->PutTo(out);
  m_switchFrame->PutTo(out);
}

MVEPulse::MVEPulse(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetModVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetModVectorElement(in, "B")) {}

MVEPulse::MVEPulse(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_aDuration(ParticleDataFactory::GetIntElement(in, "DurationA"))
, m_bDuration(ParticleDataFactory::GetIntElement(in, "DurationB"))
, m_a(ParticleDataFactory::GetModVectorElement(in, "A"))
, m_b(ParticleDataFactory::GetModVectorElement(in, "B")) {}

MVEPulse::~MVEPulse() {}

void MVEPulse::PutToInternal(athena::io::IStreamWriter& out) const {
  m_aDuration->PutTo(out);
  m_bDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

void MVEPulse::PutToInternal(nlohmann::ordered_json& out) const {
  m_aDuration->PutTo(out);
  m_bDuration->PutTo(out);
  m_a->PutTo(out);
  m_b->PutTo(out);
}

MVEImplosion::MVEImplosion(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_implosionPoint(ParticleDataFactory::GetVectorElement(in, "ImplosionPoint"))
, m_magnitudeScale(ParticleDataFactory::GetRealElement(in, "MagnitudeScale"))
, m_maxMagnitude(ParticleDataFactory::GetRealElement(in, "MaxMagnitude"))
, m_minMagnitude(ParticleDataFactory::GetRealElement(in, "MinMagnitude"))
, m_enableMinMag(ParticleDataFactory::GetBool(in, "EnableMinMag")) {}

MVEImplosion::MVEImplosion(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_implosionPoint(ParticleDataFactory::GetVectorElement(in, "ImplosionPoint"))
, m_magnitudeScale(ParticleDataFactory::GetRealElement(in, "MagnitudeScale"))
, m_maxMagnitude(ParticleDataFactory::GetRealElement(in, "MaxMagnitude"))
, m_minMagnitude(ParticleDataFactory::GetRealElement(in, "MinMagnitude"))
, m_enableMinMag(ParticleDataFactory::GetBool(in, "EnableMinMag")) {}

MVEImplosion::~MVEImplosion() {}

void MVEImplosion::PutToInternal(athena::io::IStreamWriter& out) const {
  m_implosionPoint->PutTo(out);
  m_magnitudeScale->PutTo(out);
  m_maxMagnitude->PutTo(out);
  m_minMagnitude->PutTo(out);
  ParticleDataFactory::SetBool(out, m_enableMinMag);
}

void MVEImplosion::PutToInternal(nlohmann::ordered_json& out) const {
  m_implosionPoint->PutTo(out);
  m_magnitudeScale->PutTo(out);
  m_maxMagnitude->PutTo(out);
  m_minMagnitude->PutTo(out);
  ParticleDataFactory::SetBool(out, "EnableMinMag", m_enableMinMag);
}
MVELinearImplosion::MVELinearImplosion(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_implosionPoint(ParticleDataFactory::GetVectorElement(in, "ImplosionPoint"))
, m_magnitudeScale(ParticleDataFactory::GetRealElement(in, "MagnitudeScale"))
, m_maxMagnitude(ParticleDataFactory::GetRealElement(in, "MaxMagnitude"))
, m_minMagnitude(ParticleDataFactory::GetRealElement(in, "MinMagnitude"))
, m_enableMinMag(ParticleDataFactory::GetBool(in, "EnableMinMag")) {}

MVELinearImplosion::MVELinearImplosion(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_implosionPoint(ParticleDataFactory::GetVectorElement(in, "ImplosionPoint"))
, m_magnitudeScale(ParticleDataFactory::GetRealElement(in, "MagnitudeScale"))
, m_maxMagnitude(ParticleDataFactory::GetRealElement(in, "MaxMagnitude"))
, m_minMagnitude(ParticleDataFactory::GetRealElement(in, "MinMagnitude"))
, m_enableMinMag(ParticleDataFactory::GetBool(in, "EnableMinMag")) {}

MVELinearImplosion::~MVELinearImplosion() {}

void MVELinearImplosion::PutToInternal(athena::io::IStreamWriter& out) const {
  m_implosionPoint->PutTo(out);
  m_magnitudeScale->PutTo(out);
  m_maxMagnitude->PutTo(out);
  m_minMagnitude->PutTo(out);
  ParticleDataFactory::SetBool(out, m_enableMinMag);
}

void MVELinearImplosion::PutToInternal(nlohmann::ordered_json& out) const {
  m_implosionPoint->PutTo(out);
  m_magnitudeScale->PutTo(out);
  m_maxMagnitude->PutTo(out);
  m_minMagnitude->PutTo(out);
  ParticleDataFactory::SetBool(out, "EnableMinMag", m_enableMinMag);
}

MVEExponentialImplosion::MVEExponentialImplosion(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_implosionPoint(ParticleDataFactory::GetVectorElement(in, "ImplosionPoint"))
, m_magnitudeScale(ParticleDataFactory::GetRealElement(in, "MagnitudeScale"))
, m_maxMagnitude(ParticleDataFactory::GetRealElement(in, "MaxMagnitude"))
, m_minMagnitude(ParticleDataFactory::GetRealElement(in, "MinMagnitude"))
, m_enableMinMag(ParticleDataFactory::GetBool(in, "EnableMinMag")) {}

MVEExponentialImplosion::MVEExponentialImplosion(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_implosionPoint(ParticleDataFactory::GetVectorElement(in, "ImplosionPoint"))
, m_magnitudeScale(ParticleDataFactory::GetRealElement(in, "MagnitudeScale"))
, m_maxMagnitude(ParticleDataFactory::GetRealElement(in, "MaxMagnitude"))
, m_minMagnitude(ParticleDataFactory::GetRealElement(in, "MinMagnitude"))
, m_enableMinMag(ParticleDataFactory::GetBool(in, "EnableMinMag")) {}

MVEExponentialImplosion::~MVEExponentialImplosion() {}

void MVEExponentialImplosion::PutToInternal(athena::io::IStreamWriter& out) const {
  m_implosionPoint->PutTo(out);
  m_magnitudeScale->PutTo(out);
  m_maxMagnitude->PutTo(out);
  m_minMagnitude->PutTo(out);
  ParticleDataFactory::SetBool(out, m_enableMinMag);
}

void MVEExponentialImplosion::PutToInternal(nlohmann::ordered_json& out) const {
  m_implosionPoint->PutTo(out);
  m_magnitudeScale->PutTo(out);
  m_maxMagnitude->PutTo(out);
  m_minMagnitude->PutTo(out);
  ParticleDataFactory::SetBool(out, "EnableMinMag", m_enableMinMag);
}

MVESwirl::MVESwirl(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_helixPoint(ParticleDataFactory::GetVectorElement(in, "HelixPoint"))
, m_curveBinormal(ParticleDataFactory::GetVectorElement(in, "CurveBinormal"))
, m_targetRadius(ParticleDataFactory::GetRealElement(in, "TargetRadius"))
, m_tangentialVelocity(ParticleDataFactory::GetRealElement(in, "TangentialVelocity")) {}

MVESwirl::MVESwirl(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_helixPoint(ParticleDataFactory::GetVectorElement(in, "HelixPoint"))
, m_curveBinormal(ParticleDataFactory::GetVectorElement(in, "CurveBinormal"))
, m_targetRadius(ParticleDataFactory::GetRealElement(in, "TargetRadius"))
, m_tangentialVelocity(ParticleDataFactory::GetRealElement(in, "TangentialVelocity")) {}

MVESwirl::~MVESwirl() {}

void MVESwirl::PutToInternal(nlohmann::ordered_json& out) const {
  m_helixPoint->PutTo(out);
  m_curveBinormal->PutTo(out);
  m_targetRadius->PutTo(out);
  m_tangentialVelocity->PutTo(out);
}

void MVESwirl::PutToInternal(athena::io::IStreamWriter& out) const {
  m_helixPoint->PutTo(out);
  m_curveBinormal->PutTo(out);
  m_targetRadius->PutTo(out);
  m_tangentialVelocity->PutTo(out);
}

MVEBounce::MVEBounce(athena::io::IStreamReader& in)
: ModVectorElement(in)
, m_planePoint(ParticleDataFactory::GetVectorElement(in, "PlanePoint"))
, m_planeNormal(ParticleDataFactory::GetVectorElement(in, "PlaneNormal"))
, m_friction(ParticleDataFactory::GetRealElement(in, "Friction"))
, m_restitution(ParticleDataFactory::GetRealElement(in, "Restitution"))
, m_dieOnPenetration(ParticleDataFactory::GetBool(in)) {}

MVEBounce::MVEBounce(const nlohmann::ordered_json& in)
: ModVectorElement(in)
, m_planePoint(ParticleDataFactory::GetVectorElement(in, "PlanePoint"))
, m_planeNormal(ParticleDataFactory::GetVectorElement(in, "PlaneNormal"))
, m_friction(ParticleDataFactory::GetRealElement(in, "Friction"))
, m_restitution(ParticleDataFactory::GetRealElement(in, "Restitution"))
, m_dieOnPenetration(ParticleDataFactory::GetBool(in, "DieOnPenetration")) {}

MVEBounce::~MVEBounce() {}

void MVEBounce::PutToInternal(athena::io::IStreamWriter& out) const {
  m_planePoint->PutTo(out);
  m_planeNormal->PutTo(out);
  m_friction->PutTo(out);
  m_restitution->PutTo(out);
  ParticleDataFactory::SetBool(out, m_dieOnPenetration);
}

void MVEBounce::PutToInternal(nlohmann::ordered_json& out) const {
  m_planePoint->PutTo(out);
  m_planeNormal->PutTo(out);
  m_friction->PutTo(out);
  m_restitution->PutTo(out);
  ParticleDataFactory::SetBool(out, "DieOnPenetration", m_dieOnPenetration);
}

MVESetPosition::MVESetPosition(athena::io::IStreamReader& in)
: ModVectorElement(in), m_position(ParticleDataFactory::GetVectorElement(in, "Position")) {}

MVESetPosition::MVESetPosition(const nlohmann::ordered_json& in)
: ModVectorElement(in), m_position(ParticleDataFactory::GetVectorElement(in, "Position")) {}

MVESetPosition::~MVESetPosition() {}

void MVESetPosition::PutToInternal(athena::io::IStreamWriter& out) const { m_position->PutTo(out); }

void MVESetPosition::PutToInternal(nlohmann::ordered_json& out) const { m_position->PutTo(out); }

} // namespace axdl::primedep::particles