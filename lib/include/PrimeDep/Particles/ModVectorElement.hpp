#pragma once

#include "PrimeDep/Particles/IElement.hpp"

#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>

namespace axdl::primedep::particles {
class RealElement;
class VectorElement;
class IntElement;
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
  ~MVEConstant() override;

  bool isValid() const override { return m_x && m_y && m_z; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<RealElement> m_x;
  std::unique_ptr<RealElement> m_y;
  std::unique_ptr<RealElement> m_z;
};

class MVEGravity final : public ModVectorElement {
public:
  DEFINE_ELEMENT('GRAV', "Gravity");
  explicit MVEGravity(athena::io::IStreamReader& in);
  explicit MVEGravity(const nlohmann::ordered_json& in);
  ~MVEGravity() override;

  bool isValid() const override { return !!m_acceleration; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_acceleration;
};

class MVEWind final : public ModVectorElement {
public:
  DEFINE_ELEMENT('WIND', "Wind");
  explicit MVEWind(athena::io::IStreamReader& in);
  explicit MVEWind(const nlohmann::ordered_json& in);
  ~MVEWind() override;

  bool isValid() const override { return m_velocity && m_factor; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_velocity;
  std::unique_ptr<RealElement> m_factor;
};

class MVEExplode final : public ModVectorElement {
public:
  DEFINE_ELEMENT('EXPL', "Explode");
  explicit MVEExplode(athena::io::IStreamReader& in);
  explicit MVEExplode(const nlohmann::ordered_json& in);
  ~MVEExplode() override;

  bool isValid() const override { return m_impulseMagnitude && m_fallOffFactor; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<RealElement> m_impulseMagnitude;
  std::unique_ptr<RealElement> m_fallOffFactor;
};

class MVETimeChain final : public ModVectorElement {
public:
  DEFINE_ELEMENT('CHAN', "TimeChain");
  explicit MVETimeChain(athena::io::IStreamReader& in);
  explicit MVETimeChain(const nlohmann::ordered_json& in);
  ~MVETimeChain() override;

  bool isValid() const override { return m_a && m_b && m_switchFrame; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<ModVectorElement> m_a;
  std::unique_ptr<ModVectorElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class MVEPulse final : public ModVectorElement {
public:
  DEFINE_ELEMENT('PULS', "Pulse");
  explicit MVEPulse(athena::io::IStreamReader& in);
  explicit MVEPulse(const nlohmann::ordered_json& in);
  ~MVEPulse() override;

  bool isValid() const override { return m_aDuration && m_bDuration && m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<IntElement> m_aDuration;
  std::unique_ptr<IntElement> m_bDuration;
  std::unique_ptr<ModVectorElement> m_a;
  std::unique_ptr<ModVectorElement> m_b;
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

class MVELinearImplosion final : public ModVectorElement {
public:
  DEFINE_ELEMENT('LMPL', "Implosion");

  explicit MVELinearImplosion(athena::io::IStreamReader& in);
  explicit MVELinearImplosion(const nlohmann::ordered_json& in);
  ~MVELinearImplosion() override;

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

class MVEExponentialImplosion final : public ModVectorElement {
public:
  DEFINE_ELEMENT('EMPL', "Implosion");

  explicit MVEExponentialImplosion(athena::io::IStreamReader& in);
  explicit MVEExponentialImplosion(const nlohmann::ordered_json& in);
  ~MVEExponentialImplosion() override;

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

class MVESwirl final : public ModVectorElement {
public:
  DEFINE_ELEMENT('SWRL', "Swirl");
  explicit MVESwirl(athena::io::IStreamReader& in);
  explicit MVESwirl(const nlohmann::ordered_json& in);
  ~MVESwirl() override;

  bool isValid() const override { return m_helixPoint && m_curveBinormal && m_targetRadius && m_tangentialVelocity; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<VectorElement> m_helixPoint;
  std::unique_ptr<VectorElement> m_curveBinormal;
  std::unique_ptr<RealElement> m_targetRadius;
  std::unique_ptr<RealElement> m_tangentialVelocity;
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

class MVESetPosition final : public ModVectorElement {
public:
  DEFINE_ELEMENT('SPOS', "SetPosition");
  explicit MVESetPosition(athena::io::IStreamReader& in);
  explicit MVESetPosition(const nlohmann::ordered_json& in);
  ~MVESetPosition() override;

  bool isValid() const override { return !!m_position; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_position;
};
} // namespace axdl::primedep::particles