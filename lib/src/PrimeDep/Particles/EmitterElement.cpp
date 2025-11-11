#include "PrimeDep/Particles/EmitterElement.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {
EESimpleEmitter::EESimpleEmitter(athena::io::IStreamReader& in) : EmitterElement(in) {
  if (ParticleDataFactory::GetClassID(in) == FOURCC('ILOC')) {
    m_initialLocation.reset(ParticleDataFactory::GetVectorElement(in, "InitialLocation"));
    if (ParticleDataFactory::GetClassID(in) == FOURCC('IVEC')) {
      m_vector.reset(ParticleDataFactory::GetVectorElement(in, "Vector"));
    }
  }
}

EESimpleEmitter::EESimpleEmitter(const nlohmann::ordered_json& in)
: EmitterElement(in)
, m_initialLocation(ParticleDataFactory::GetVectorElement(in, "InitialLocation"))
, m_vector(ParticleDataFactory::GetVectorElement(in, "Vector")) {}

EESimpleEmitter::~EESimpleEmitter() {}

void EESimpleEmitter::PutToInternal(athena::io::IStreamWriter& out) const {
  if (m_initialLocation) {
    ParticleDataFactory::SetClassID(out, FOURCC('ILOC'));
    m_initialLocation->PutTo(out);
    if (m_vector) {
      ParticleDataFactory::SetClassID(out, FOURCC('IVEC'));
      m_vector->PutTo(out);
    }
  }
}

void EESimpleEmitter::PutToInternal(nlohmann::ordered_json& out) const {
  ParticleDataFactory::SetClassID(out, "SimpleEmitter");

  if (m_initialLocation) {
    m_initialLocation->PutTo(out);
    if (m_vector) {
      m_vector->PutTo(out);
    }
  }
}

} // namespace axdl::primedep::particles