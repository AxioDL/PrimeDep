#pragma once

#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/Particles/IElement.hpp"

namespace axdl::primedep::particles {
class IntElement;
class UVElement : public IElement {
public:
  explicit UVElement(athena::io::IStreamReader& reader) : IElement(reader) {}
  explicit UVElement(const nlohmann::ordered_json& reader) : IElement(reader) {}

  virtual std::optional<AssetId32Big> textureId() const { return std::nullopt; };
};

class UVENone : public UVElement {
public:
  DEFINE_ELEMENT('NONE', "None");
  explicit UVENone(athena::io::IStreamReader& reader) : UVElement(reader) {}
  explicit UVENone(const nlohmann::ordered_json& reader) : UVElement(reader) {}

  bool isValid() const override { return true; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override {}
  void PutToInternal(nlohmann::ordered_json& out) const override {}
};

class UVEConstant32Big final : public UVElement {
public:
  DEFINE_ELEMENT('CNST', "Constant");
  explicit UVEConstant32Big(athena::io::IStreamReader& reader);
  explicit UVEConstant32Big(const nlohmann::ordered_json& reader);
  bool isValid() const override { return true; }

  std::optional<AssetId32Big> textureId() const override { return m_textureId; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;

protected:
  std::optional<AssetId32Big> m_textureId;
};

class UVEAnimated32Big final : public UVElement {
public:
  DEFINE_ELEMENT('ATEX', "AnimatedTexture");
  explicit UVEAnimated32Big(athena::io::IStreamReader& reader);
  explicit UVEAnimated32Big(const nlohmann::ordered_json& reader);
  ~UVEAnimated32Big() override;

  bool isValid() const override {
    return m_tileWidth && m_tileHeight && m_strideWidth && m_strideHeight && m_cycleFrames;
  }

  std::optional<AssetId32Big> textureId() const override { return m_textureId; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  std::optional<AssetId32Big> m_textureId;
  std::unique_ptr<IntElement> m_tileWidth;
  std::unique_ptr<IntElement> m_tileHeight;
  std::unique_ptr<IntElement> m_strideWidth;
  std::unique_ptr<IntElement> m_strideHeight;
  std::unique_ptr<IntElement> m_cycleFrames;
  bool m_loop;
};
} // namespace axdl::primedep::particles