#pragma once

#include "PrimeDep/Math/Vector3f.hpp"
#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class RealElement;
class IntElement;
class ColorElement;

class VectorElement : public IElement {
public:
  explicit VectorElement(athena::io::IStreamReader& in) : IElement(in) {}
  explicit VectorElement(const nlohmann::ordered_json& in) : IElement(in) {}
};

class VENone final : public VectorElement {
public:
  DEFINE_ELEMENT('NONE', "None");
  explicit VENone(athena::io::IStreamReader& in) : VectorElement(in) {}
  explicit VENone(const nlohmann::ordered_json& in) : VectorElement(in) {}
  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class VEConstant final : public VectorElement {
public:
  DEFINE_ELEMENT('CNST', "Constant");
  explicit VEConstant(athena::io::IStreamReader& in);
  explicit VEConstant(const nlohmann::ordered_json& in);
  ~VEConstant() override;

  bool isValid() const override { return m_x && m_y && m_z; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_x;
  std::unique_ptr<RealElement> m_y;
  std::unique_ptr<RealElement> m_z;
};

class VEKeyframeEmitter final : public VectorElement {
public:
  explicit VEKeyframeEmitter(athena::io::IStreamReader& in);
  explicit VEKeyframeEmitter(const nlohmann::ordered_json& in);

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
  std::vector<Vector3f> m_keys;
};

class VECone final : public VectorElement {
public:
  DEFINE_ELEMENT('CONE', "Cone");
  explicit VECone(athena::io::IStreamReader& in);
  explicit VECone(const nlohmann::ordered_json& in);
  ~VECone() override;

  bool isValid() const override { return m_direction && m_baseRadius; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_direction;
  std::unique_ptr<RealElement> m_baseRadius;
};

class VEAngleCone final : public VectorElement {
public:
  DEFINE_ELEMENT('ANGC', "AngleCone");
  explicit VEAngleCone(athena::io::IStreamReader& in);
  explicit VEAngleCone(const nlohmann::ordered_json& in);
  ~VEAngleCone() override;

  bool isValid() const override {
    return m_angleConstantX && m_angleConstantY && m_angleXRange && m_angleYRange && m_magnitude;
  }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<RealElement> m_angleConstantX;
  std::unique_ptr<RealElement> m_angleConstantY;
  std::unique_ptr<RealElement> m_angleXRange;
  std::unique_ptr<RealElement> m_angleYRange;
  std::unique_ptr<RealElement> m_magnitude;
};

class VECircle final : public VectorElement {
public:
  DEFINE_ELEMENT('CIRC', "Circle");
  explicit VECircle(athena::io::IStreamReader& in);
  explicit VECircle(const nlohmann::ordered_json& in);
  ~VECircle() override;

  bool isValid() const override {
    return m_circleOffset && m_circleNormal && m_angleConstant && m_angleLinear && m_radius;
  }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<VectorElement> m_circleOffset;
  std::unique_ptr<VectorElement> m_circleNormal;
  std::unique_ptr<RealElement> m_angleConstant;
  std::unique_ptr<RealElement> m_angleLinear;
  std::unique_ptr<RealElement> m_radius;
};

class VECircleCluster final : public VectorElement {
public:
  DEFINE_ELEMENT('CCLU', "CircleCluster");
  explicit VECircleCluster(athena::io::IStreamReader& in);
  explicit VECircleCluster(const nlohmann::ordered_json& in);
  ~VECircleCluster() override;

  bool isValid() const override { return m_circleOffset && m_circleNormal && m_cycleFrames && m_randomFactor; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_circleOffset;
  std::unique_ptr<VectorElement> m_circleNormal;
  std::unique_ptr<IntElement> m_cycleFrames;
  std::unique_ptr<RealElement> m_randomFactor;
};

class VEAdd final : public VectorElement {
public:
  DEFINE_ELEMENT('ADD_', "Add");

  explicit VEAdd(athena::io::IStreamReader& in);
  explicit VEAdd(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_a;
  std::unique_ptr<VectorElement> m_b;
};

class VEMultiply final : public VectorElement {
public:
  DEFINE_ELEMENT('MULT', "Multiply");
  explicit VEMultiply(athena::io::IStreamReader& in);
  explicit VEMultiply(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_a;
  std::unique_ptr<VectorElement> m_b;
};

class VETimeChain final : public VectorElement {
public:
  DEFINE_ELEMENT('CHAN', "TimeChain");
  explicit VETimeChain(athena::io::IStreamReader& in);
  explicit VETimeChain(const nlohmann::ordered_json& in);
  ~VETimeChain() override;

  bool isValid() const override { return m_a && m_b && m_switchFrame; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_a;
  std::unique_ptr<VectorElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class VEPulse final : public VectorElement {
public:
  DEFINE_ELEMENT('PULS', "Pulse");
  explicit VEPulse(athena::io::IStreamReader& in);
  explicit VEPulse(const nlohmann::ordered_json& in);
  ~VEPulse() override;

  bool isValid() const override { return m_aDuration && m_bDuration && m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<IntElement> m_aDuration;
  std::unique_ptr<IntElement> m_bDuration;
  std::unique_ptr<VectorElement> m_a;
  std::unique_ptr<VectorElement> m_b;
};

class VERealToVector final : public VectorElement {
public:
  DEFINE_ELEMENT('RTOV', "RealToVector");
  explicit VERealToVector(athena::io::IStreamReader& in);
  explicit VERealToVector(const nlohmann::ordered_json& in);
  ~VERealToVector() override;

  bool isValid() const override { return !!m_value; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<RealElement> m_value;
};

class VEParticleLocation final : public VectorElement {
public:
  DEFINE_ELEMENT('PLOC', "ParticleLocation");
  explicit VEParticleLocation(athena::io::IStreamReader& in) : VectorElement(in) {}
  explicit VEParticleLocation(const nlohmann::ordered_json& in) : VectorElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class VEParticleColor final : public VectorElement {
public:
  DEFINE_ELEMENT('PLCO', "ParticleColor");
  explicit VEParticleColor(athena::io::IStreamReader& in) : VectorElement(in) {}
  explicit VEParticleColor(const nlohmann::ordered_json& in) : VectorElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class VEParticleVelocity final : public VectorElement {
public:
  DEFINE_ELEMENT('PVEL', "ParticleVelocity");
  explicit VEParticleVelocity(athena::io::IStreamReader& in) : VectorElement(in) {}
  explicit VEParticleVelocity(const nlohmann::ordered_json& in) : VectorElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class VEParticleSystemOrientationFront final : public VectorElement {
public:
  DEFINE_ELEMENT('PSOF', "ParticleSystemOrientationFront");
  explicit VEParticleSystemOrientationFront(athena::io::IStreamReader& in) : VectorElement(in) {}
  explicit VEParticleSystemOrientationFront(const nlohmann::ordered_json& in) : VectorElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class VEParticleSystemOrientationUp final : public VectorElement {
public:
  DEFINE_ELEMENT('PSOU', "ParticleSystemOrientationUp");
  explicit VEParticleSystemOrientationUp(athena::io::IStreamReader& in) : VectorElement(in) {}
  explicit VEParticleSystemOrientationUp(const nlohmann::ordered_json& in) : VectorElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class VEParticleSystemOrientationRight final : public VectorElement {
public:
  DEFINE_ELEMENT('PSOR', "ParticleSystemOrientationRight");
  explicit VEParticleSystemOrientationRight(athena::io::IStreamReader& in) : VectorElement(in) {}
  explicit VEParticleSystemOrientationRight(const nlohmann::ordered_json& in) : VectorElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class VEParticleSystemTranslation final : public VectorElement {
public:
  DEFINE_ELEMENT('PSTR', "ParticleSystemTranslation");
  explicit VEParticleSystemTranslation(athena::io::IStreamReader& in) : VectorElement(in) {}
  explicit VEParticleSystemTranslation(const nlohmann::ordered_json& in) : VectorElement(in) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class VESubtract final : public VectorElement {
public:
  DEFINE_ELEMENT('SUB_', "Subtract");
  explicit VESubtract(athena::io::IStreamReader& in);
  explicit VESubtract(const nlohmann::ordered_json& in);

  bool isValid() const override { return m_a && m_b; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_a;
  std::unique_ptr<VectorElement> m_b;
};

class VEColorToVector final : public VectorElement {
public:
  DEFINE_ELEMENT('CTVC', "ColorToVector");
  explicit VEColorToVector(athena::io::IStreamReader& in);
  explicit VEColorToVector(const nlohmann::ordered_json& in);
  ~VEColorToVector() override;
  bool isValid() const override { return !!m_value; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<ColorElement> m_value;
};
} // namespace axdl::primedep::particles