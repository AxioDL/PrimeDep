#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class IntElement;
class VectorElement;
class ColorElement;
class RealElement : public IElement {
public:
  explicit RealElement(athena::io::IStreamReader& in) : IElement(in) {}
  explicit RealElement(const nlohmann::ordered_json& in) : IElement(in) {}
};

class RENone final : public RealElement {
public:
  DEFINE_ELEMENT('NONE', "None");
  explicit RENone(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit RENone(const nlohmann::ordered_json& in) : RealElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REConstant final : public RealElement {
public:
  DEFINE_ELEMENT('CNST', "Constant");
  explicit REConstant(athena::io::IStreamReader& in);
  explicit REConstant(const nlohmann::ordered_json& in);

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  float m_value;
};

class REKeyframeEmitter final : public RealElement {
public:
  explicit REKeyframeEmitter(athena::io::IStreamReader& in);
  explicit REKeyframeEmitter(const nlohmann::ordered_json& in);

  static constexpr FourCC ClassIDPercent() { return FOURCC('KEYP'); }
  static constexpr FourCC ClassIDNormal() { return FOURCC('KEYE'); }

  static constexpr std::string_view ClassName() { return "KeyframeEmitter"sv; }
  constexpr FourCC classId() const override { return m_percent ? ClassIDPercent() : ClassIDNormal(); }
  constexpr std::string_view className() const override { return ClassName(); }

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

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
  DEFINE_ELEMENT('SCAL', "TimeScale");
  explicit RETimeScale(athena::io::IStreamReader& in);
  explicit RETimeScale(const nlohmann::ordered_json& in);
  ~RETimeScale() override;

  bool isValid() const override { return !!m_scale; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_scale;
};

class RESineWave final : public RealElement {
public:
  DEFINE_ELEMENT('SINE', "SineWave");
  explicit RESineWave(athena::io::IStreamReader& in);
  explicit RESineWave(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_frequency && m_amplitude && m_phase; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_frequency;
  std::unique_ptr<RealElement> m_amplitude;
  std::unique_ptr<RealElement> m_phase;
};

class REAdd final : public RealElement {
public:
  DEFINE_ELEMENT('ADD_', "Add");
  explicit REAdd(athena::io::IStreamReader& in);
  explicit REAdd(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class REMultiply final : public RealElement {
public:
  DEFINE_ELEMENT('MULT', "Multiply");
  explicit REMultiply(athena::io::IStreamReader& in);
  explicit REMultiply(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class REDotProduct final : public RealElement {
public:
  DEFINE_ELEMENT('DOTP', "DotProduct");
  explicit REDotProduct(athena::io::IStreamReader& in);
  explicit REDotProduct(const nlohmann::ordered_json& in);
  ~REDotProduct() override;

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_a;
  std::unique_ptr<VectorElement> m_b;
};

class RERandom final : public RealElement {
public:
  DEFINE_ELEMENT('RAND', "Random");
  explicit RERandom(athena::io::IStreamReader& in);
  explicit RERandom(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_min && m_max; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_min;
  std::unique_ptr<RealElement> m_max;
};

class REInitialRandom final : public RealElement {
public:
  DEFINE_ELEMENT('IRND', "InitialRandom");
  explicit REInitialRandom(athena::io::IStreamReader& in);
  explicit REInitialRandom(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_min && m_max; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_min;
  std::unique_ptr<RealElement> m_max;
};

class RETimeChain final : public RealElement {
public:
  DEFINE_ELEMENT('CHAN', "TimeChain");
  explicit RETimeChain(athena::io::IStreamReader& in);
  explicit RETimeChain(const nlohmann::ordered_json& in);
  ~RETimeChain() override;

  bool isValid() const override { return m_a && m_b && m_switchFrame; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class REClamp final : public RealElement {
public:
  DEFINE_ELEMENT('CLMP', "Clamp");
  explicit REClamp(athena::io::IStreamReader& in);
  explicit REClamp(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_min && m_max && m_value; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_min;
  std::unique_ptr<RealElement> m_max;
  std::unique_ptr<RealElement> m_value;
};

class REPulse final : public RealElement {
public:
  DEFINE_ELEMENT('PULS', "Pulse");
  explicit REPulse(athena::io::IStreamReader& in);
  explicit REPulse(const nlohmann::ordered_json& in);
  ~REPulse() override;

  bool isValid() const override { return m_aDuration && m_bDuration && m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_aDuration;
  std::unique_ptr<IntElement> m_bDuration;
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class RELifetimePercent final : public RealElement {
public:
  DEFINE_ELEMENT('RLPT', "LifetimePercent");
  explicit RELifetimePercent(athena::io::IStreamReader& in);
  explicit RELifetimePercent(const nlohmann::ordered_json& in);

  bool isValid() const override { return !!m_lifetime; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_lifetime;
};

class RELifetimeTween final : public RealElement {
public:
  DEFINE_ELEMENT('LFTW', "LifetimeTween");
  explicit RELifetimeTween(athena::io::IStreamReader& in);
  explicit RELifetimeTween(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class REParticleRotationOrLineWidth final : public RealElement {
public:
  DEFINE_ELEMENT('PRLW', "ParticleRotationOrLineWidth");

  explicit REParticleRotationOrLineWidth(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleRotationOrLineWidth(const nlohmann::ordered_json& in) : RealElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REParticleSizeOrLineLength final : public RealElement {
public:
  DEFINE_ELEMENT('PSLL', "ParticleSizeOrLineLength");
  explicit REParticleSizeOrLineLength(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleSizeOrLineLength(const nlohmann::ordered_json& in) : RealElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REParticleAccessParameter1 final : public RealElement {
public:
  DEFINE_ELEMENT('PAP1', "ParticleAccessParameter1");
  explicit REParticleAccessParameter1(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleAccessParameter1(const nlohmann::ordered_json& in) : RealElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REParticleAccessParameter2 final : public RealElement {
public:
  DEFINE_ELEMENT('PAP2', "ParticleAccessParameter2");
  explicit REParticleAccessParameter2(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleAccessParameter2(const nlohmann::ordered_json& in) : RealElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REParticleAccessParameter3 final : public RealElement {
public:
  DEFINE_ELEMENT('PAP3', "ParticleAccessParameter3");
  explicit REParticleAccessParameter3(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleAccessParameter3(const nlohmann::ordered_json& in) : RealElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REParticleAccessParameter4 final : public RealElement {
public:
  DEFINE_ELEMENT('PAP4', "ParticleAccessParameter4");
  explicit REParticleAccessParameter4(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleAccessParameter4(const nlohmann::ordered_json& in) : RealElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class REParticleAccessParameter5 final : public RealElement {
public:
  DEFINE_ELEMENT('PAP5', "ParticleAccessParameter5");
  explicit REParticleAccessParameter5(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleAccessParameter5(const nlohmann::ordered_json& in) : RealElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REParticleAccessParameter6 final : public RealElement {
public:
  DEFINE_ELEMENT('PAP6', "ParticleAccessParameter6");
  explicit REParticleAccessParameter6(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleAccessParameter6(const nlohmann::ordered_json& in) : RealElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REParticleAccessParameter7 final : public RealElement {
public:
  DEFINE_ELEMENT('PAP7', "ParticleAccessParameter7");
  explicit REParticleAccessParameter7(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleAccessParameter7(const nlohmann::ordered_json& in) : RealElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REParticleAccessParameter8 final : public RealElement {
public:
  DEFINE_ELEMENT('PAP8', "ParticleAccessParameter8");
  explicit REParticleAccessParameter8(athena::io::IStreamReader& in) : RealElement(in) {}
  explicit REParticleAccessParameter8(const nlohmann::ordered_json& in) : RealElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class REVectorXToReal final : public RealElement {
public:
  DEFINE_ELEMENT('VXTR', "VectorXToReal");

  explicit REVectorXToReal(athena::io::IStreamReader& in);
  explicit REVectorXToReal(const nlohmann::ordered_json& in);
  ~REVectorXToReal() override;
  bool isValid() const override { return !!m_vector; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_vector;
};

class REVectorYToReal final : public RealElement {
public:
  DEFINE_ELEMENT('VYTR', "VectorYToReal");

  explicit REVectorYToReal(athena::io::IStreamReader& in);
  explicit REVectorYToReal(const nlohmann::ordered_json& in);
  ~REVectorYToReal() override;

  bool isValid() const override { return !!m_vector; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_vector;
};

class REVectorZToReal final : public RealElement {
public:
  DEFINE_ELEMENT('VZTR', "VectorZToReal");

  explicit REVectorZToReal(athena::io::IStreamReader& in);
  explicit REVectorZToReal(const nlohmann::ordered_json& in);
  ~REVectorZToReal() override;
  bool isValid() const override { return !!m_vector; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_vector;
};

class REVectorMagnitude final : public RealElement {
public:
  DEFINE_ELEMENT('VMAG', "VectorMagnitude");

  explicit REVectorMagnitude(athena::io::IStreamReader& in);
  explicit REVectorMagnitude(const nlohmann::ordered_json& in);
  ~REVectorMagnitude() override;
  bool isValid() const override { return !!m_vector; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_vector;
};

class REInitialSwitch final : public RealElement {
public:
  DEFINE_ELEMENT('ISWT', "InitialSwitch");
  explicit REInitialSwitch(athena::io::IStreamReader& in);
  explicit REInitialSwitch(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class RECompareLessThan final : public RealElement {
public:
  DEFINE_ELEMENT('CLTN', "CompareLessThan");
  explicit RECompareLessThan(athena::io::IStreamReader& in);
  explicit RECompareLessThan(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b && m_c && m_d; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<RealElement> m_c;
  std::unique_ptr<RealElement> m_d;
};

class RECompareEqual final : public RealElement {
public:
  DEFINE_ELEMENT('CEQL', "CompareEqual");

  explicit RECompareEqual(athena::io::IStreamReader& in);
  explicit RECompareEqual(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b && m_c && m_d; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<RealElement> m_c;
  std::unique_ptr<RealElement> m_d;
};

class REConstantRange final : public RealElement {
public:
  DEFINE_ELEMENT('CRNG', "ConstantRange");
  explicit REConstantRange(athena::io::IStreamReader& in);
  explicit REConstantRange(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b && m_c && m_d && m_e; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<RealElement> m_c;
  std::unique_ptr<RealElement> m_d;
  std::unique_ptr<RealElement> m_e;
};

class REExternalVar final : public RealElement {
public:
  DEFINE_ELEMENT('CEXT', "ExternalVar");
  explicit REExternalVar(athena::io::IStreamReader& in);
  explicit REExternalVar(const nlohmann::ordered_json& in);
  ~REExternalVar() override;

  bool isValid() const override { return !!m_value; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_value;
};

class REIntTimesReal final : public RealElement {
public:
  DEFINE_ELEMENT('ITRL', "IntTimesReal");
  explicit REIntTimesReal(athena::io::IStreamReader& in);
  explicit REIntTimesReal(const nlohmann::ordered_json& in);
  ~REIntTimesReal() override;

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class RESubtract final : public RealElement {
public:
  DEFINE_ELEMENT('SUB_', "Subtract");
  explicit RESubtract(athena::io::IStreamReader& in);
  explicit RESubtract(const nlohmann::ordered_json& in);
  ~RESubtract() override;

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class REGetComponentRed final : public RealElement {
public:
  DEFINE_ELEMENT('GTCR', "GetComponentRed");
  explicit REGetComponentRed(athena::io::IStreamReader& in);
  explicit REGetComponentRed(const nlohmann::ordered_json& in);
  ~REGetComponentRed() override;

  bool isValid() const override { return !!m_color; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<ColorElement> m_color;
};

class REGetComponentGreen final : public RealElement {
public:
  DEFINE_ELEMENT('GTCG', "GetComponentGreen");

  explicit REGetComponentGreen(athena::io::IStreamReader& in);
  explicit REGetComponentGreen(const nlohmann::ordered_json& in);
  ~REGetComponentGreen() override;

  bool isValid() const override { return !!m_color; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<ColorElement> m_color;
};

class REGetComponentBlue final : public RealElement {
public:
  DEFINE_ELEMENT('GTCB', "GetComponentBlue");

  explicit REGetComponentBlue(athena::io::IStreamReader& in);
  explicit REGetComponentBlue(const nlohmann::ordered_json& in);
  ~REGetComponentBlue() override;

  bool isValid() const override { return !!m_color; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<ColorElement> m_color;
};

class REGetComponentAlpha final : public RealElement {
public:
  DEFINE_ELEMENT('GTCA', "GetComponentAlpha");
  explicit REGetComponentAlpha(athena::io::IStreamReader& in);
  explicit REGetComponentAlpha(const nlohmann::ordered_json& in);
  ~REGetComponentAlpha() override;

  bool isValid() const override { return !!m_color; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<ColorElement> m_color;
};
} // namespace axdl::primedep::particles