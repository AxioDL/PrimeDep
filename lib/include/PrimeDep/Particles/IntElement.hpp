#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class IntElement : public IElement {
public:
  void PutTo(athena::io::IStreamWriter& out) override {}
  void PutTo(nlohmann::ordered_json& out) override {}
  virtual int value() = 0;
};
} // namespace axdl::primedep::particles