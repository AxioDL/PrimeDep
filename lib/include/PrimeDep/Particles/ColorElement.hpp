#pragma once

#include "PrimeDep/Math/Color.hpp"
#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class IntElement;
class RealElement;
class ColorElement : public IElement {
public:
  explicit ColorElement(athena::io::IStreamReader& in) : IElement(in) {}
  explicit ColorElement(const nlohmann::ordered_json& in) : IElement(in) {}
};

class CENone final : public ColorElement {
public:
  DEFINE_ELEMENT('NONE', "None");
  explicit CENone(athena::io::IStreamReader& in) : ColorElement(in) {}
  explicit CENone(const nlohmann::ordered_json& in) : ColorElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};

class CEConstant final : public ColorElement {
public:
  DEFINE_ELEMENT('CNST', "Constant");
  explicit CEConstant(athena::io::IStreamReader& in);
  explicit CEConstant(const nlohmann::ordered_json& in);
  ~CEConstant() override;
  bool isValid() const override { return m_r && m_g && m_b && m_a; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_r;
  std::unique_ptr<RealElement> m_g;
  std::unique_ptr<RealElement> m_b;
  std::unique_ptr<RealElement> m_a;
};

class CEKeyframeEmitter final : public ColorElement {
public:
  explicit CEKeyframeEmitter(athena::io::IStreamReader& in);
  explicit CEKeyframeEmitter(const nlohmann::ordered_json& in);

  bool isValid() const override { return true; }

  static constexpr FourCC ClassIDPercent() { return FOURCC('KEYP'); }
  static constexpr FourCC ClassIDNormal() { return FOURCC('KEYE'); }

  static constexpr std::string_view ClassName() { return "KeyframeEmitter"sv; }
  constexpr FourCC classId() const override { return m_percent ? ClassIDPercent() : ClassIDNormal(); }
  constexpr std::string_view className() const override { return ClassName(); }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

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
  DEFINE_ELEMENT('CHAN', "TimeChain");
  explicit CETimeChain(athena::io::IStreamReader& in);
  explicit CETimeChain(const nlohmann::ordered_json& in);
  ~CETimeChain() override;

  bool isValid() const override { return m_a && m_b && m_switchFrame; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<ColorElement> m_a;
  std::unique_ptr<ColorElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class CEFadeEnd final : public ColorElement {
public:
  DEFINE_ELEMENT('CFDE', "FadeEnd");
  explicit CEFadeEnd(athena::io::IStreamReader& in);
  explicit CEFadeEnd(const nlohmann::ordered_json& in);
  ~CEFadeEnd() override;

  bool isValid() const override { return m_a && m_b && m_startFrame && m_endFrame; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<ColorElement> m_a;
  std::unique_ptr<ColorElement> m_b;
  std::unique_ptr<RealElement> m_startFrame;
  std::unique_ptr<RealElement> m_endFrame;
};

class CEFade final : public ColorElement {
public:
  DEFINE_ELEMENT('FADE', "Fade");
  explicit CEFade(athena::io::IStreamReader& in);
  explicit CEFade(const nlohmann::ordered_json& in);
  ~CEFade() override;

  bool isValid() const override { return m_a && m_b && m_endFrame; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<ColorElement> m_a;
  std::unique_ptr<ColorElement> m_b;
  std::unique_ptr<RealElement> m_endFrame;
};

class CEPulse final : public ColorElement {
public:
  DEFINE_ELEMENT('PULS', "Pulse");
  explicit CEPulse(athena::io::IStreamReader& in);
  explicit CEPulse(const nlohmann::ordered_json& in);
  ~CEPulse() override;

  bool isValid() const override { return m_aDuration && m_bDuration && m_a && m_b; }

private:
  auto PutToInternal(athena::io::IStreamWriter& out) const -> void override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<IntElement> m_aDuration;
  std::unique_ptr<IntElement> m_bDuration;
  std::unique_ptr<ColorElement> m_a;
  std::unique_ptr<ColorElement> m_b;
};

class CEParticleColor final : public ColorElement {
public:
  DEFINE_ELEMENT('PCOL', "ParticleColor");
  explicit CEParticleColor(athena::io::IStreamReader& in) : ColorElement(in) {}
  explicit CEParticleColor(const nlohmann::ordered_json& in) : ColorElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {};
  void PutToInternal(nlohmann::ordered_json& out) const override {};
};
} // namespace axdl::primedep::particles