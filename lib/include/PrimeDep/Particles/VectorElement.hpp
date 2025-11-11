#pragma once

#include "PrimeDep/Math/Vector3f.hpp"
#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class RealElement;
class IntElement;

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

class VECircleCluster final : public VectorElement {
public:
  DEFINE_ELEMENT('CCLU', "CircleCluster");
  explicit VECircleCluster(athena::io::IStreamReader& in);
  explicit VECircleCluster(const nlohmann::ordered_json& in);
  ~VECircleCluster() override;

  bool isValid() const override { return m_direction && m_upVector && m_deltaAngle && m_magnitude; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_direction;
  std::unique_ptr<VectorElement> m_upVector;
  std::unique_ptr<IntElement> m_deltaAngle;
  std::unique_ptr<RealElement> m_magnitude;
};

class VECircle final : public VectorElement {
public:
  DEFINE_ELEMENT('CIRC', "Circle");
  explicit VECircle(athena::io::IStreamReader& in);
  explicit VECircle(const nlohmann::ordered_json& in);
  ~VECircle() override;

  bool isValid() const override { return m_direction && m_upVector && m_angleConstant && m_angleLinear && m_radius; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::unique_ptr<VectorElement> m_direction;
  std::unique_ptr<VectorElement> m_upVector;
  std::unique_ptr<RealElement> m_angleConstant;
  std::unique_ptr<RealElement> m_angleLinear;
  std::unique_ptr<RealElement> m_radius;
};

} // namespace axdl::primedep::particles