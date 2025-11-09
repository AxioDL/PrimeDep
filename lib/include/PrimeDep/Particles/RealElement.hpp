#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class IntElement;
class VectorElement;
class ColorElement;
class RealElement : public IElement {};

class RENone final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REConstant final : public RealElement {
public:
  explicit REConstant(athena::io::IStreamReader& in);
  explicit REConstant(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  float m_value;
};

class REKeyframeEmitter final : public RealElement {
public:
  explicit REKeyframeEmitter(athena::io::IStreamReader& in);
  explicit REKeyframeEmitter(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  uint32_t m_percent{};
  uint32_t m_unknown1{};
  bool m_loop{};
  bool m_unknown2{};
  int32_t m_loopEnd{};
  int32_t m_loopStart{};
  std::vector<float> m_keys;
};

class RETimeScale final : public RealElement {
public:
  explicit RETimeScale(athena::io::IStreamReader& in);
  explicit RETimeScale(const nlohmann::ordered_json& in);
  ~RETimeScale() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_scale;
};

class RESineWave final : public RealElement {
public:
  explicit RESineWave(athena::io::IStreamReader& in);
  explicit RESineWave(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_frequency;
  std::unique_ptr<RealElement> m_amplitude;
  std::unique_ptr<RealElement> m_phase;
};

class REAdd final : public RealElement {
public:
  explicit REAdd(athena::io::IStreamReader& in);
  explicit REAdd(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class REMultiply final : public RealElement {
public:
  explicit REMultiply(athena::io::IStreamReader& in);
  explicit REMultiply(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class REDotProduct final : public RealElement {
public:
  explicit REDotProduct(athena::io::IStreamReader& in);
  explicit REDotProduct(const nlohmann::ordered_json& in);
  ~REDotProduct() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_a;
  std::unique_ptr<VectorElement> m_b;
};

class RERandom final : public RealElement {
public:
  explicit RERandom(athena::io::IStreamReader& in);
  explicit RERandom(const nlohmann::ordered_json& in);
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_min;
  std::unique_ptr<RealElement> m_max;
};

class REInitialRandom final : public RealElement {
public:
  explicit REInitialRandom(athena::io::IStreamReader& in);
  explicit REInitialRandom(const nlohmann::ordered_json& in);
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_min;
  std::unique_ptr<RealElement> m_max;
};

class RETimeChain final : public RealElement {
public:
  explicit RETimeChain(athena::io::IStreamReader& in);
  explicit RETimeChain(const nlohmann::ordered_json& in);
  ~RETimeChain() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class REClamp final : public RealElement {
public:
  explicit REClamp(athena::io::IStreamReader& in);
  explicit REClamp(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_min;
  std::unique_ptr<RealElement> m_max;
  std::unique_ptr<RealElement> m_value;
};

class REPulse final : public RealElement {
public:
  explicit REPulse(athena::io::IStreamReader& in);
  explicit REPulse(const nlohmann::ordered_json& in);
  ~REPulse() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_aDuration;
  std::unique_ptr<IntElement> m_bDuration;
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class RELifetimePercent final : public RealElement {
public:
  explicit RELifetimePercent(athena::io::IStreamReader& in);
  explicit RELifetimePercent(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_lifetime;
};

class RELifetimeTween final : public RealElement {
public:
  explicit RELifetimeTween(athena::io::IStreamReader& in);
  explicit RELifetimeTween(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class REParticleRotationOrLineWidth final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleSizeOrLineLength final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleAccessParameter1 final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleAccessParameter2 final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleAccessParameter3 final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleAccessParameter4 final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleAccessParameter5 final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleAccessParameter6 final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleAccessParameter7 final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REParticleAccessParameter8 final : public RealElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class REVectorXToReal final : public RealElement {
public:
  explicit REVectorXToReal(athena::io::IStreamReader& in);
  explicit REVectorXToReal(const nlohmann::ordered_json& in);
  ~REVectorXToReal() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_vector;
};

class REVectorYToReal final : public RealElement {
public:
  explicit REVectorYToReal(athena::io::IStreamReader& in);
  explicit REVectorYToReal(const nlohmann::ordered_json& in);
  ~REVectorYToReal() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_vector;
};

class REVectorZToReal final : public RealElement {
public:
  explicit REVectorZToReal(athena::io::IStreamReader& in);
  explicit REVectorZToReal(const nlohmann::ordered_json& in);
  ~REVectorZToReal() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_vector;
};

class REVectorMagnitude final : public RealElement {
public:
  explicit REVectorMagnitude(athena::io::IStreamReader& in);
  explicit REVectorMagnitude(const nlohmann::ordered_json& in);
  ~REVectorMagnitude() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_vector;
};

class RECompareLessThan final : public RealElement {
public:
  explicit RECompareLessThan(athena::io::IStreamReader& in);
  explicit RECompareLessThan(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<RealElement> m_c;
  std::unique_ptr<RealElement> m_d;
};

class RECompareEqual final : public RealElement {
public:
  explicit RECompareEqual(athena::io::IStreamReader& in);
  explicit RECompareEqual(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<RealElement> m_c;
  std::unique_ptr<RealElement> m_d;
};

class REConstantRange final : public RealElement {
public:
  explicit REConstantRange(athena::io::IStreamReader& in);
  explicit REConstantRange(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<RealElement> m_c;
  std::unique_ptr<RealElement> m_d;
  std::unique_ptr<RealElement> m_e;
};

class REExternalVar final : public RealElement {
public:
  explicit REExternalVar(athena::io::IStreamReader& in);
  explicit REExternalVar(const nlohmann::ordered_json& in);
  ~REExternalVar() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_value;
};

class REIntTimesReal final : public RealElement {
public:
  explicit REIntTimesReal(athena::io::IStreamReader& in);
  explicit REIntTimesReal(const nlohmann::ordered_json& in);
  ~REIntTimesReal() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class RESubtract final : public RealElement {
public:
  explicit RESubtract(athena::io::IStreamReader& in);
  explicit RESubtract(const nlohmann::ordered_json& in);
  ~RESubtract() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class REGetComponentRed final : public RealElement {
public:
  explicit REGetComponentRed(athena::io::IStreamReader& in);
  explicit REGetComponentRed(const nlohmann::ordered_json& in);
  ~REGetComponentRed() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<ColorElement> m_color;
};

class REGetComponentGreen final : public RealElement {
public:
  explicit REGetComponentGreen(athena::io::IStreamReader& in);
  explicit REGetComponentGreen(const nlohmann::ordered_json& in);
  ~REGetComponentGreen() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<ColorElement> m_color;
};

class REGetComponentBlue final : public RealElement {
public:
  explicit REGetComponentBlue(athena::io::IStreamReader& in);
  explicit REGetComponentBlue(const nlohmann::ordered_json& in);
  ~REGetComponentBlue() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<ColorElement> m_color;
};

class REGetComponentAlpha final : public RealElement {
public:
  explicit REGetComponentAlpha(athena::io::IStreamReader& in);
  explicit REGetComponentAlpha(const nlohmann::ordered_json& in);
  ~REGetComponentAlpha() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<ColorElement> m_color;
};
} // namespace axdl::primedep::particles