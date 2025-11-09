#include "PrimeDep/Particles/ModVectorElement.hpp"

#include "PrimeDep/FourCC.hpp"
#include "PrimeDep/Particles/ParticleData.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {

MVEConstant::MVEConstant(athena::io::IStreamReader& in)
: m_x(ParticleDataFactory::GetRealElement(in))
, m_y(ParticleDataFactory::GetRealElement(in))
, m_z(ParticleDataFactory::GetRealElement(in)) {}

MVEConstant::MVEConstant(const nlohmann::ordered_json& in)
: m_x(ParticleDataFactory::GetRealElement(in, "X"))
, m_y(ParticleDataFactory::GetRealElement(in, "Y"))
, m_z(ParticleDataFactory::GetRealElement(in, "Z")) {}

void MVEConstant::PutTo(athena::io::IStreamWriter& out) const {
  // ModVector Elements must have all 3 components specified
  if (!m_x || !m_y || !m_z) {
    return;
  }
  ParticleDataFactory::SetClassID(out, FOURCC('CNST'));
  m_x->PutTo(out);
  m_y->PutTo(out);
  m_z->PutTo(out);
}

void MVEConstant::PutTo(nlohmann::ordered_json& out) const {
  // ModVector Elements must have all 3 components specified
  if (!m_x || !m_y || !m_z) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "ModVectorConstant"sv);
  m_x->PutTo(out["X"]);
  m_y->PutTo(out["Y"]);
  m_z->PutTo(out["Z"]);
}

MVEImplosion::MVEImplosion(athena::io::IStreamReader& in)
: m_implosionPoint(ParticleDataFactory::GetVectorElement(in))
, m_magnitudeScale(ParticleDataFactory::GetRealElement(in))
, m_maxMagnitude(ParticleDataFactory::GetRealElement(in))
, m_minMagnitude(ParticleDataFactory::GetRealElement(in))
, m_enableMinMag(ParticleDataFactory::GetBool(in)) {}

MVEImplosion::MVEImplosion(const nlohmann::ordered_json& in)
: m_implosionPoint(ParticleDataFactory::GetVectorElement(in, "ImplosionPoint"))
, m_magnitudeScale(ParticleDataFactory::GetRealElement(in, "MagnitudeScale"))
, m_maxMagnitude(ParticleDataFactory::GetRealElement(in, "MaxMagnitude"))
, m_minMagnitude(ParticleDataFactory::GetRealElement(in, "MinMagnitude"))
, m_enableMinMag(ParticleDataFactory::GetBool(in, "EnableMinMag")) {}

void MVEImplosion::PutTo(athena::io::IStreamWriter& out) const {
  if (!m_implosionPoint || !m_magnitudeScale || !m_maxMagnitude || !m_minMagnitude) {
    return;
  }

  ParticleDataFactory::SetClassID(out, FOURCC('IMPL'));
  m_implosionPoint->PutTo(out);
  m_magnitudeScale->PutTo(out);
  m_maxMagnitude->PutTo(out);
  m_minMagnitude->PutTo(out);
  ParticleDataFactory::SetBool(out, m_enableMinMag);
}

void MVEImplosion::PutTo(nlohmann::ordered_json& out) const {
  if (!m_implosionPoint || !m_magnitudeScale || !m_maxMagnitude) {
    return;
  }
  ParticleDataFactory::SetClassID(out, "ModVectorImplosion");
  m_implosionPoint->PutTo(out["ImplosionPoint"]);
  m_magnitudeScale->PutTo(out["MagnitudeScale"]);
  m_maxMagnitude->PutTo(out["MaxMagnitude"]);
  m_minMagnitude->PutTo(out["MinMagnitude"]);
  ParticleDataFactory::SetBool(out, "EnableMinMag", m_enableMinMag);
}

} // namespace axdl::primedep::particles