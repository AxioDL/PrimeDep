#include "PrimeDep/Particles/EmitterElement.hpp"

#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
EESimpleEmitter::EESimpleEmitter(athena::io::IStreamReader& in) : EmitterElement(in) {
  if (ParticleDataFactory::PeekClassID(in) == FOURCC('ILOC')) {
    m_isOldSpec = true;
    ParticleDataFactory::GetClassID(in);
    m_initialLocation.reset(ParticleDataFactory::GetVectorElement(in, "InitialLocation"));
    if (ParticleDataFactory::GetClassID(in) == FOURCC('IVEC')) {
      m_vector.reset(ParticleDataFactory::GetVectorElement(in, "Vector"));
    }
  } else {
    m_initialLocation.reset(ParticleDataFactory::GetVectorElement(in, "InitialLocation"));
    m_vector.reset(ParticleDataFactory::GetVectorElement(in, "Vector"));
  }
}

EESimpleEmitter::EESimpleEmitter(const nlohmann::ordered_json& in)
: EmitterElement(in)
, m_isOldSpec(in.value("OldSpec", false))
, m_initialLocation(ParticleDataFactory::GetVectorElement(in, "InitialLocation"))
, m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

EESimpleEmitter::~EESimpleEmitter() {}

void EESimpleEmitter::PutToInternal(athena::io::IStreamWriter& out) const {
  if (m_isOldSpec) {
    if (m_initialLocation) {
      ParticleDataFactory::SetClassID(out, FOURCC('ILOC'));
      m_initialLocation->PutTo(out);
      if (m_vector) {
        ParticleDataFactory::SetClassID(out, FOURCC('IVEC'));
        m_vector->PutTo(out);
      }
    }
    return;
  }

  m_initialLocation->PutTo(out);
  m_vector->PutTo(out);
}

void EESimpleEmitter::PutToInternal(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "SimpleEmitter");
  if (m_isOldSpec) {
    out["OldSpec"] = m_isOldSpec;
    if (m_initialLocation) {
      m_initialLocation->PutTo(out);
      if (m_vector) {
        m_vector->PutTo(out);
      }
    }
  } else {
    m_initialLocation->PutTo(out);
    m_vector->PutTo(out);
  }
}

VESphere::VESphere(athena::io::IStreamReader& in)
: EmitterElement(in)
, m_sphereOrigin(ParticleDataFactory::GetVectorElement(in, "SphereOrigin"))
, m_sphereRadius(ParticleDataFactory::GetRealElement(in, "SphereRadius"))
, m_velocity(ParticleDataFactory::GetRealElement(in, "Velocity")) {}

VESphere::VESphere(const nlohmann::ordered_json& in)
: EmitterElement(in)
, m_sphereOrigin(ParticleDataFactory::GetVectorElement(in, "SphereOrigin"))
, m_sphereRadius(ParticleDataFactory::GetRealElement(in, "SphereRadius"))
, m_velocity(ParticleDataFactory::GetRealElement(in, "Velocity")) {}

VESphere::~VESphere() {}

void VESphere::PutToInternal(athena::io::IStreamWriter& out) const {
  m_sphereOrigin->PutTo(out);
  m_sphereRadius->PutTo(out);
  m_velocity->PutTo(out);
}

void VESphere::PutToInternal(nlohmann::ordered_json& out) const {
  m_sphereOrigin->PutTo(out);
  m_sphereRadius->PutTo(out);
  m_velocity->PutTo(out);
}

VEAngleSphere::VEAngleSphere(athena::io::IStreamReader& in)
: EmitterElement(in)
, m_sphereOrigin(ParticleDataFactory::GetVectorElement(in, "SphereOrigin"))
, m_sphereRadius(ParticleDataFactory::GetRealElement(in, "SphereRadius"))
, m_velocity(ParticleDataFactory::GetRealElement(in, "Velocity"))
, m_angleXBias(ParticleDataFactory::GetRealElement(in, "AngleXBias"))
, m_angleYBias(ParticleDataFactory::GetRealElement(in, "AngleYBias"))
, m_angleXRange(ParticleDataFactory::GetRealElement(in, "AngleXRange"))
, m_angleYRange(ParticleDataFactory::GetRealElement(in, "AngleYRange")) {}

VEAngleSphere::VEAngleSphere(const nlohmann::ordered_json& in)
: EmitterElement(in)
, m_sphereOrigin(ParticleDataFactory::GetVectorElement(in, "SphereOrigin"))
, m_sphereRadius(ParticleDataFactory::GetRealElement(in, "SphereRadius"))
, m_velocity(ParticleDataFactory::GetRealElement(in, "Velocity"))
, m_angleXBias(ParticleDataFactory::GetRealElement(in, "AngleXBias"))
, m_angleYBias(ParticleDataFactory::GetRealElement(in, "AngleYBias"))
, m_angleXRange(ParticleDataFactory::GetRealElement(in, "AngleXRange"))
, m_angleYRange(ParticleDataFactory::GetRealElement(in, "AngleYRange")) {}

VEAngleSphere::~VEAngleSphere() {}

void VEAngleSphere::PutToInternal(athena::io::IStreamWriter& out) const {
  m_sphereOrigin->PutTo(out);
  m_sphereRadius->PutTo(out);
  m_velocity->PutTo(out);
  m_angleXBias->PutTo(out);
  m_angleYBias->PutTo(out);
  m_angleXRange->PutTo(out);
  m_angleYRange->PutTo(out);
}

void VEAngleSphere::PutToInternal(nlohmann::ordered_json& out) const {
  m_sphereOrigin->PutTo(out);
  m_sphereRadius->PutTo(out);
  m_velocity->PutTo(out);
  m_angleXBias->PutTo(out);
  m_angleYBias->PutTo(out);
  m_angleXRange->PutTo(out);
  m_angleYRange->PutTo(out);
}

} // namespace axdl::primedep::particles