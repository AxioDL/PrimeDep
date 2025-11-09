#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class RealElement;
class IntElement : public IElement {};

class IENone final : public IntElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class IEConstant final : public IntElement {
public:
  explicit IEConstant(athena::io::IStreamReader& in);
  explicit IEConstant(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  int m_value{};
};

class IEKeyframeEmitter final : public IntElement {
public:
  explicit IEKeyframeEmitter(athena::io::IStreamReader& in);
  explicit IEKeyframeEmitter(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
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
  explicit IETimeScale(athena::io::IStreamReader& in);
  explicit IETimeScale(const nlohmann::ordered_json& in);
  ~IETimeScale() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_scale;
};

class IEDeath final : public IntElement {
public:
  explicit IEDeath(athena::io::IStreamReader& in);
  explicit IEDeath(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IETimeChain final : public IntElement {
public:
  explicit IETimeChain(athena::io::IStreamReader& in);
  explicit IETimeChain(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class IEAdd final : public IntElement {
public:
  explicit IEAdd(athena::io::IStreamReader& in);
  explicit IEAdd(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IEMultiply final : public IntElement {
public:
  explicit IEMultiply(athena::io::IStreamReader& in);
  explicit IEMultiply(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IEModulo final : public IntElement {
public:
  explicit IEModulo(athena::io::IStreamReader& in);
  explicit IEModulo(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IERandom final : public IntElement {
public:
  explicit IERandom(athena::io::IStreamReader& in);
  explicit IERandom(const nlohmann::ordered_json& in);
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_min;
  std::unique_ptr<IntElement> m_max;
};

class IEImpulse final : public IntElement {
public:
  explicit IEImpulse(athena::io::IStreamReader& in);
  explicit IEImpulse(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_impulse;
};

class IELifetimePercent final : public IntElement {
public:
  explicit IELifetimePercent(athena::io::IStreamReader& in);
  explicit IELifetimePercent(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_lifetime;
};

class IESampleAndHold final : public IntElement {
public:
  explicit IESampleAndHold(athena::io::IStreamReader& in);
  explicit IESampleAndHold(const nlohmann::ordered_json& in);
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_sampleSource;
  // int m_nextSampleFrame;
  std::unique_ptr<IntElement> m_waitFramesMin;
  std::unique_ptr<IntElement> m_waitFramesMax;
  // int m_holdVal;
};

class IEInitialRandom final : public IntElement {
public:
  explicit IEInitialRandom(athena::io::IStreamReader& in);
  explicit IEInitialRandom(const nlohmann::ordered_json& in);
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_min;
  std::unique_ptr<IntElement> m_max;
};

class IEClamp final : public IntElement {
public:
  explicit IEClamp(athena::io::IStreamReader& in);
  explicit IEClamp(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_min;
  std::unique_ptr<IntElement> m_max;
  std::unique_ptr<IntElement> m_value;
};

class IEPulse final : public IntElement {
public:
  explicit IEPulse(athena::io::IStreamReader& in);
  explicit IEPulse(const nlohmann::ordered_json& in);
  ~IEPulse() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_aDuration;
  std::unique_ptr<IntElement> m_bDuration;
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IERealToInt final : public IntElement {
public:
  explicit IERealToInt(athena::io::IStreamReader& in);
  explicit IERealToInt(const nlohmann::ordered_json& in);
  ~IERealToInt() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_a;
  std::unique_ptr<RealElement> m_b;
};

class IESubtract final : public IntElement {
public:
  explicit IESubtract(athena::io::IStreamReader& in);
  explicit IESubtract(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_a;
  std::unique_ptr<IntElement> m_b;
};

class IEGetCumulativeParticleCount final : public IntElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class IEGetActiveParticleCount final : public IntElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class IEGetEmitterTime final : public IntElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};
} // namespace axdl::primedep::particles