#pragma once

#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class VectorElement;
class RealElement;
class EmitterElement : public IElement {
public:
  explicit EmitterElement(athena::io::IStreamReader& reader) : IElement(reader) {}
  explicit EmitterElement(const nlohmann::ordered_json& reader) : IElement(reader) {}
};

class EENone : public EmitterElement {
public:
  DEFINE_ELEMENT('NONE', "None");
  explicit EENone(athena::io::IStreamReader& reader) : EmitterElement(reader) {}
  explicit EENone(const nlohmann::ordered_json& reader) : EmitterElement(reader) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class EESimpleEmitter final : public EmitterElement {
public:
  explicit EESimpleEmitter(athena::io::IStreamReader& reader);
  explicit EESimpleEmitter(const nlohmann::ordered_json& reader);
  ~EESimpleEmitter() override;

  static constexpr FourCC ClassIDNew() { return FOURCC('SEMR'); }
  static constexpr FourCC ClassIDOld() { return FOURCC('SETR'); }
  static constexpr std::string_view ClassName() { return "SimpleEmitter"sv; }
  constexpr FourCC classId() const override { return m_isOldSpec ? ClassIDOld() : ClassIDNew(); }
  constexpr std::string_view className() const override { return ClassName(); }

  bool isValid() const override {
    if (m_isOldSpec) {
      return !!m_initialLocation;
    }

    return m_initialLocation && m_vector;
  }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  bool m_isOldSpec{false};
  std::unique_ptr<VectorElement> m_initialLocation;
  std::unique_ptr<VectorElement> m_vector;
};

class VESphere final : public EmitterElement {
public:
  DEFINE_ELEMENT('SPHE', "Sphere");
  explicit VESphere(athena::io::IStreamReader& in);
  explicit VESphere(const nlohmann::ordered_json& in);

  ~VESphere() override;

  bool isValid() const override { return m_sphereOrigin && m_sphereRadius && m_velocity; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_sphereOrigin;
  std::unique_ptr<RealElement> m_sphereRadius;
  std::unique_ptr<RealElement> m_velocity;
};

class VEAngleSphere final : public EmitterElement {
public:
  DEFINE_ELEMENT('ASPH', "AngleSphere");
  explicit VEAngleSphere(athena::io::IStreamReader& in);
  explicit VEAngleSphere(const nlohmann::ordered_json& in);
  ~VEAngleSphere() override;

  bool isValid() const override {
    return m_sphereOrigin && m_sphereRadius && m_velocity && m_angleXBias && m_angleYBias && m_angleXRange &&
           m_angleYRange;
  }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

  std::unique_ptr<VectorElement> m_sphereOrigin;
  std::unique_ptr<RealElement> m_sphereRadius;
  std::unique_ptr<RealElement> m_velocity;
  std::unique_ptr<RealElement> m_angleXBias;
  std::unique_ptr<RealElement> m_angleYBias;
  std::unique_ptr<RealElement> m_angleXRange;
  std::unique_ptr<RealElement> m_angleYRange;
};
} // namespace axdl::primedep::particles