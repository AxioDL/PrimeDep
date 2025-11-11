#include "PrimeDep/Particles/ModVectorElement.hpp"

#include "PrimeDep/FourCC.hpp"
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

} // namespace axdl::primedep::particles