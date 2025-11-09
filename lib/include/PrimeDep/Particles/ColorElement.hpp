#pragma once

#include "PrimeDep/Math/Color.hpp"
#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class IntElement;
class RealElement;
class ColorElement : public IElement {};

class CENone final : public ColorElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class CEConstant final : public ColorElement {
public:
  explicit CEConstant(athena::io::IStreamReader& in);
  explicit CEConstant(const nlohmann::ordered_json& in);
  ~CEConstant() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_r;
  std::unique_ptr<RealElement> m_g;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<RealElement> m_a;
};

class CEKeyframeEmitter final : public ColorElement {
public:
  explicit CEKeyframeEmitter(athena::io::IStreamReader& in);
  explicit CEKeyframeEmitter(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  uint32_t m_percent{};
  uint32_t m_unknown1{};
  bool m_loop{};
  bool m_unknown2{};
  int32_t m_loopEnd{};
  int32_t m_loopStart{};
  std::vector<Color> m_keys;
};

class CETimeChain final : public ColorElement {
public:
  explicit CETimeChain(athena::io::IStreamReader& in);
  explicit CETimeChain(const nlohmann::ordered_json& in);
  ~CETimeChain() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<ColorElement> m_a;
  std::unique_ptr<ColorElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class CEFadeEnd final : public ColorElement {
public:
  explicit CEFadeEnd(athena::io::IStreamReader& in);
  explicit CEFadeEnd(const nlohmann::ordered_json& in);
  ~CEFadeEnd() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<ColorElement> m_a;
  std::unique_ptr<ColorElement> m_b;
  std::unique_ptr<RealElement> m_startFrame;
  std::unique_ptr<RealElement> m_endFrame;
};

class CEFade final : public ColorElement {
public:
  explicit CEFade(athena::io::IStreamReader& in);
  explicit CEFade(const nlohmann::ordered_json& in);
  ~CEFade() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<ColorElement> m_a;
  std::unique_ptr<ColorElement> m_b;
  std::unique_ptr<RealElement> m_endFrame;
};

class CEPulse final : public ColorElement {
public:
  explicit CEPulse(athena::io::IStreamReader& in);
  explicit CEPulse(const nlohmann::ordered_json& in);
  ~CEPulse() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<IntElement> m_aDuration;
  std::unique_ptr<IntElement> m_bDuration;
  std::unique_ptr<ColorElement> m_a;
  std::unique_ptr<ColorElement> m_b;
};

class CEParticleColor final : public ColorElement {
public:
  explicit CEParticleColor(athena::io::IStreamReader& in);
  explicit CEParticleColor(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};
} // namespace axdl::primedep::particles