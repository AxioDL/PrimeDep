#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class RealElement;
class IntElement : public IElement {
public:
  explicit IntElement(athena::io::IStreamReader& in) : IElement(in) {}
  explicit IntElement(const nlohmann::ordered_json& in) : IElement(in) {}
};

class IENone final : public IntElement {
public:
  DEFINE_ELEMENT('NONE', "None");
  explicit IENone(athena::io::IStreamReader& in) : IntElement(in) {}
  explicit IENone(const nlohmann::ordered_json& in) : IntElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class IEConstant final : public IntElement {
public:
  DEFINE_ELEMENT('CNST', "Constant");
  explicit IEConstant(athena::io::IStreamReader& in);
  explicit IEConstant(const nlohmann::ordered_json& in);

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  int m_value{};
};

class IEKeyframeEmitter final : public IntElement {
public:
  explicit IEKeyframeEmitter(athena::io::IStreamReader& in);
  explicit IEKeyframeEmitter(const nlohmann::ordered_json& in);

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
  std::vector<int> m_keys;
};

class IETimeScale final : public IntElement {
public:
  DEFINE_ELEMENT('TSCL', "TimeScale");
  explicit IETimeScale(athena::io::IStreamReader& in);
  explicit IETimeScale(const nlohmann::ordered_json& in);
  ~IETimeScale() override;

  bool isValid() const override { return !!m_scale; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_scale;
};

class IEDeath final : public IntElement {
public:
  DEFINE_ELEMENT('DETH', "Death");
  explicit IEDeath(athena::io::IStreamReader& in);
  explicit IEDeath(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IETimeChain final : public IntElement {
public:
  DEFINE_ELEMENT('CHAN', "TimeChain");
  explicit IETimeChain(athena::io::IStreamReader& in);
  explicit IETimeChain(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b && m_switchFrame; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class IEAdd final : public IntElement {
public:
  DEFINE_ELEMENT('ADD_', "Add");
  explicit IEAdd(athena::io::IStreamReader& in);
  explicit IEAdd(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IEMultiply final : public IntElement {
public:
  DEFINE_ELEMENT('MULT', "Multiply");
  explicit IEMultiply(athena::io::IStreamReader& in);
  explicit IEMultiply(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IEModulo final : public IntElement {
public:
  DEFINE_ELEMENT('MODU', "Modulo");
  explicit IEModulo(athena::io::IStreamReader& in);
  explicit IEModulo(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IERandom final : public IntElement {
public:
  DEFINE_ELEMENT('RAND', "Random");
  explicit IERandom(athena::io::IStreamReader& in);
  explicit IERandom(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_min && m_max; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_min;
  std::unique_ptr<IntElement> m_max;
};

class IEImpulse final : public IntElement {
public:
  DEFINE_ELEMENT('IMPL', "Impulse");
  explicit IEImpulse(athena::io::IStreamReader& in);
  explicit IEImpulse(const nlohmann::ordered_json& in);

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_impulse;
};

class IELifetimePercent final : public IntElement {
public:
  DEFINE_ELEMENT('ILPT', "LifetimePercent");
  explicit IELifetimePercent(athena::io::IStreamReader& in);
  explicit IELifetimePercent(const nlohmann::ordered_json& in);

  bool isValid() const override { return !!m_lifetime; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_lifetime;
};

class IESampleAndHold final : public IntElement {
public:
  DEFINE_ELEMENT('SPAH', "SampleAndHold");
  explicit IESampleAndHold(athena::io::IStreamReader& in);
  explicit IESampleAndHold(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_sampleSource && m_waitFramesMin && m_waitFramesMax; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<IntElement> m_sampleSource;
  // int m_nextSampleFrame;
  std::unique_ptr<IntElement> m_waitFramesMin;
  std::unique_ptr<IntElement> m_waitFramesMax;
  // int m_holdVal;
};

class IEInitialRandom final : public IntElement {
public:
  DEFINE_ELEMENT('IRND', "InitialRandom");
  explicit IEInitialRandom(athena::io::IStreamReader& in);
  explicit IEInitialRandom(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_min && m_max; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_min;
  std::unique_ptr<IntElement> m_max;
};

class IEClamp final : public IntElement {
public:
  DEFINE_ELEMENT('CLMP', "Clamp");
  explicit IEClamp(athena::io::IStreamReader& in);
  explicit IEClamp(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_min && m_max && m_value; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_min;
  std::unique_ptr<IntElement> m_max;
  std::unique_ptr<IntElement> m_value;
};

class IEPulse final : public IntElement {
public:
  DEFINE_ELEMENT('PULS', "Pulse");
  explicit IEPulse(athena::io::IStreamReader& in);
  explicit IEPulse(const nlohmann::ordered_json& in);
  ~IEPulse() override;

  bool isValid() const override { return m_aDuration && m_bDuration && m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_aDuration;
  std::unique_ptr<IntElement> m_bDuration;
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IERealToInt final : public IntElement {
public:
  DEFINE_ELEMENT('RTOI', "RealToInt");
  explicit IERealToInt(athena::io::IStreamReader& in);
  explicit IERealToInt(const nlohmann::ordered_json& in);
  ~IERealToInt() override;

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class IESubtract final : public IntElement {
public:
  DEFINE_ELEMENT('SUB_', "Subtract");
  explicit IESubtract(athena::io::IStreamReader& in);
  explicit IESubtract(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IEGetCumulativeParticleCount final : public IntElement {
public:
  DEFINE_ELEMENT('GTCP', "GetCumulativeParticleCount");
  explicit IEGetCumulativeParticleCount(athena::io::IStreamReader& in) : IntElement(in) {}
  explicit IEGetCumulativeParticleCount(const nlohmann::ordered_json& in) : IntElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class IEGetActiveParticleCount final : public IntElement {
public:
  DEFINE_ELEMENT('GAPC', "GetActiveParticleCount");
  explicit IEGetActiveParticleCount(athena::io::IStreamReader& in) : IntElement(in) {}
  explicit IEGetActiveParticleCount(const nlohmann::ordered_json& in) : IntElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class IEGetEmitterTime final : public IntElement {
public:
  DEFINE_ELEMENT('GEMT', "GetEmitterTime");
  explicit IEGetEmitterTime(athena::io::IStreamReader& in) : IntElement(in) {}
  explicit IEGetEmitterTime(const nlohmann::ordered_json& in) : IntElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};
} // namespace axdl::primedep::particles