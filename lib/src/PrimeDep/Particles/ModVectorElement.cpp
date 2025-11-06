#include "PrimeDep/Particles/ModVectorElement.hpp"

#include "PrimeDep/FourCC.hpp"
#include "PrimeDep/Particles/ParticleDataFactory.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include <nlohmann/json.hpp>

namespace axdl::primedep::particles {

MVEConstant::MVEConstant(athena::io::IStreamReader& in) {
  m_x.reset(ParticleDataFactory::GetRealElement(in));
  m_y.reset(ParticleDataFactory::GetRealElement(in));
  m_z.reset(ParticleDataFactory::GetRealElement(in));
}

void MVEConstant::PutTo(athena::io::IStreamWriter& out) const {
  // ModVector Elements must have all 3 components specified
  if (!m_x || !m_y || !m_z) {
    return;
  }
  FOURCC('CNST').PutTo(out);
  m_x->PutTo(out);
  m_y->PutTo(out);
  m_z->PutTo(out);
}

void MVEConstant::PutTo(nlohmann::ordered_json& out) const {
  // ModVector Elements must have all 3 components specified
  if (!m_x || !m_y || !m_z) {
    return;
  }
  out["Type"] = "MVEConstant";
  m_x->PutTo(out["X"]);
  m_y->PutTo(out["Y"]);
  m_z->PutTo(out["Z"]);
}
} // namespace axdl::primedep::particles