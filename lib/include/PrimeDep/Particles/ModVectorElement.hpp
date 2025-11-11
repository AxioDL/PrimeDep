#pragma once

#include "PrimeDep/Particles/IElement.hpp"

#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>

namespace axdl::primedep::particles {
class RealElement;
class VectorElement;
class ModVectorElement : public IElement {
public:
  explicit ModVectorElement(athena::io::IStreamReader& in) : IElement(in) {}
  explicit ModVectorElement(const nlohmann::ordered_json& in) : IElement(in) {}
};

class MVENone final : public ModVectorElement {
public:
  DEFINE_ELEMENT('NONE', "None");
  explicit MVENone(athena::io::IStreamReader& in) : ModVectorElement(in) {}
  explicit MVENone(const nlohmann::ordered_json& in) : ModVectorElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class MVEConstant final : public ModVectorElement {
public:
  DEFINE_ELEMENT('CNST', "Constant");
  explicit MVEConstant(athena::io::IStreamReader& in);
  explicit MVEConstant(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_x && m_y && m_z; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<RealElement> m_x;
  std::unique_ptr<RealElement> m_y;
  std::unique_ptr<RealElement> m_z;
};

class MVEImplosion final : public ModVectorElement {
public:
  DEFINE_ELEMENT('IMPL', "Implosion");

  explicit MVEImplosion(athena::io::IStreamReader& in);
  explicit MVEImplosion(const nlohmann::ordered_json& in);
  ~MVEImplosion() override;

  bool isValid() const override { return m_implosionPoint && m_magnitudeScale && m_maxMagnitude && m_minMagnitude; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_implosionPoint;
  std::unique_ptr<RealElement> m_magnitudeScale;
  std::unique_ptr<RealElement> m_maxMagnitude;
  std::unique_ptr<RealElement> m_minMagnitude;
  bool m_enableMinMag{false};
};

class MVEBounce final : public ModVectorElement {
public:
  DEFINE_ELEMENT('BNCE', "Bounce");
  explicit MVEBounce(athena::io::IStreamReader& in);
  explicit MVEBounce(const nlohmann::ordered_json& in);
  ~MVEBounce() override;

  bool isValid() const override { return m_planePoint && m_planeNormal && m_friction && m_restitution; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_planePoint;
  std::unique_ptr<VectorElement> m_planeNormal;
  std::unique_ptr<RealElement> m_friction;
  std::unique_ptr<RealElement> m_restitution;
  bool m_dieOnPenetration;
};
} // namespace axdl::primedep::particles