#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class VectorElement;
class EmitterElement : public IElement {
public:
  explicit EmitterElement(athena::io::IStreamReader& reader) : IElement(reader) {}
  explicit EmitterElement(const nlohmann::ordered_json& reader) : IElement(reader) {}
};

class EESimpleEmitter final : public EmitterElement {
public:
  DEFINE_ELEMENT('SETR', "SimpleEmitter");
  explicit EESimpleEmitter(athena::io::IStreamReader& reader);
  explicit EESimpleEmitter(const nlohmann::ordered_json& reader);
  ~EESimpleEmitter() override;

  bool isValid() const override { return !!m_initialLocation; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_initialLocation;
  std::unique_ptr<VectorElement> m_vector;
};
} // namespace axdl::primedep::particles