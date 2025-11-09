#pragma once

#include "PrimeDep/Particles/IElement.hpp"

#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>

namespace axdl::primedep::particles {
class RealElement;
class VectorElement;
class ModVectorElement : public IElement {
public:
};

class MVEConstant final : public ModVectorElement {
public:
  explicit MVEConstant(athena::io::IStreamReader& in);
  explicit MVEConstant(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_x;
  std::unique_ptr<RealElement> m_y;
  std::unique_ptr<RealElement> m_z;
};

class MVEImplosion final : public ModVectorElement {
public:
  explicit MVEImplosion(athena::io::IStreamReader& in);
  explicit MVEImplosion(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_implosionPoint;
  std::unique_ptr<RealElement> m_magnitudeScale;
  std::unique_ptr<RealElement> m_maxMagnitude;
  std::unique_ptr<RealElement> m_minMagnitude;
  bool m_enableMinMag{false};
};
} // namespace axdl::primedep::particles