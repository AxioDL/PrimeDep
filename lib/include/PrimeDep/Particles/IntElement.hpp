#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class IntElement : public IElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override {}
  void PutTo(nlohmann::ordered_json& out) const override {}
};
} // namespace axdl::primedep::particles