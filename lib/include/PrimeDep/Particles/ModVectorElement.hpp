#pragma once

#include "PrimeDep/Particles/IElement.hpp"

#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>

namespace axdl::primedep::particles {
class RealElement;
class ModVectorElement : public IElement {
public:
};

class MVEConstant final : public ModVectorElement {
public:
  MVEConstant() = default;
  MVEConstant(athena::io::IStreamReader& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_x;
  std::unique_ptr<RealElement> m_y;
  std::unique_ptr<RealElement> m_z;
};
} // namespace axdl::primedep::particles