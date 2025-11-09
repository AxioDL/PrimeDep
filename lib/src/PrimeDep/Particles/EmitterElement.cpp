#include "PrimeDep/Particles/EmitterElement.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
EESimpleEmitter::EESimpleEmitter(athena::io::IStreamReader& in) {
  if (ParticleDataFactory::GetClassID(in) == FOURCC('ILOC')) {
    m_initialLocation.reset(ParticleDataFactory::GetVectorElement(in));
    if (ParticleDataFactory::GetClassID(in) == FOURCC('IVEC')) {
      m_vector.reset(ParticleDataFactory::GetVectorElement(in));
    }
  }
}

EESimpleEmitter::EESimpleEmitter(const nlohmann::ordered_json& in)
: m_initialLocation(ParticleDataFactory::GetVectorElement(in, "InitialLocation"))
, m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

void EESimpleEmitter::PutTo(athena::io::IStreamWriter& out) const {
  if (m_initialLocation) {
    ParticleDataFactory::SetClassID(out, FOURCC('ILOC'));
    m_initialLocation->PutTo(out);
    if (m_vector) {
      ParticleDataFactory::SetClassID(out, FOURCC('IVEC'));
      m_vector->PutTo(out);
    }
  }
}

void EESimpleEmitter::PutTo(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "SimpleEmitter");

  if (m_initialLocation) {
    m_initialLocation->PutTo(out["InitialLocation"]);
  }

  if (m_vector) {
    m_vector->PutTo(out["Vector"]);
  }
}

} // namespace axdl::primedep::particles