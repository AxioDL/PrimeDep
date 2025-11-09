#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class VectorElement;
class EmitterElement : public IElement {};

class EESimpleEmitter final : public EmitterElement {
public:
  explicit EESimpleEmitter(athena::io::IStreamReader& in);
  explicit EESimpleEmitter(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_initialLocation;
  std::unique_ptr<VectorElement> m_vector;
};
} // namespace axdl::primedep::particles