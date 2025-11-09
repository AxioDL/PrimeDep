#pragma once

#include "PrimeDep/Math/Vector3f.hpp"
#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class RealElement;
class IntElement;

class VectorElement : public IElement {
public:
};

class VENone final : public VectorElement {
public:
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;
};

class VEConstant final : public VectorElement {
public:
  explicit VEConstant(athena::io::IStreamReader& in);
  explicit VEConstant(const nlohmann::ordered_json& in);
  ~VEConstant() override;
  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<RealElement> m_x;
  std::unique_ptr<RealElement> m_y;
  std::unique_ptr<RealElement> m_z;
};

class VEKeyframeEmitter final : public VectorElement {
public:
  explicit VEKeyframeEmitter(athena::io::IStreamReader& in);
  explicit VEKeyframeEmitter(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
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
  explicit VETimeChain(athena::io::IStreamReader& in);
  explicit VETimeChain(const nlohmann::ordered_json& in);
  ~VETimeChain() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_a;
  std::unique_ptr<VectorElement> m_b;
  std::unique_ptr<IntElement> m_switchFrame;
};

class VEBounce final : public VectorElement {
public:
  explicit VEBounce(athena::io::IStreamReader& in);
  explicit VEBounce(const nlohmann::ordered_json& in);
  ~VEBounce() override;

  void PutTo(athena::io::IStreamWriter& out) const override;
  void PutTo(nlohmann::ordered_json& out) const override;

private:
  std::unique_ptr<VectorElement> m_planePoint;
  std::unique_ptr<VectorElement> m_planeNormal;
  std::unique_ptr<RealElement> m_friction;
  std::unique_ptr<RealElement> m_restitution;
  bool m_dieOnPenetration;
};

} // namespace axdl::primedep::particles