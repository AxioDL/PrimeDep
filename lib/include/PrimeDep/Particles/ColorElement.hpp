#pragma once

#include "PrimeDep/Math/Color.hpp"
#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class RealElement;
class ColorElement : public IElement {
public:
};

class CEConstant final : public ColorElement {
public:
  explicit CEConstant(athena::io::IStreamReader& in);
  explicit CEConstant(nlohmann::ordered_json& in);

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

} // namespace axdl::primedep::particles