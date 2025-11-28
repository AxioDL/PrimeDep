#pragma once

#include "PrimeDep/IResource.hpp"

namespace axdl::primedep ::MetroidPrime {
class RasterFont final : public TypedResource('FONT', ".rpfr", ".rpff", DESCRIPTION("Raster Font")) {
public:
  class Glyph {
  public:
    Glyph() = default;
    explicit Glyph(athena::io::IStreamReader& in, uint32_t version);
    explicit Glyph(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out, uint32_t version) const;
    void PutTo(nlohmann::ordered_json& out, uint32_t version) const;

    [[nodiscard]] int16_t leftPadding() const { return m_leftPadding; }
    void setLeftPadding(const int16_t leftPadding) { m_leftPadding = leftPadding; }

    [[nodiscard]] int16_t advance() const { return m_advance; }
    void setAdvance(const int16_t advance) { m_advance = advance; }

    [[nodiscard]] int16_t rightPadding() const { return m_rightPadding; }
    void setRightPadding(const int16_t rightPadding) { m_rightPadding = rightPadding; }

    [[nodiscard]] float startU() const { return m_startU; }
    void setStartU(const float startU) { m_startU = startU; }

    [[nodiscard]] float startV() const { return m_startV; }
    void setStartV(const float startV) { m_startV = startV; }

    [[nodiscard]] float endU() const { return m_endU; }
    void setEndU(const float endU) { m_endU = endU; }

    [[nodiscard]] float endV() const { return m_endV; }
    void setEndV(const float endV) { m_endV = endV; }

    [[nodiscard]] int16_t cellWidth() const { return m_cellWidth; }
    void setCellWidth(const int16_t cellWidth) { m_cellWidth = cellWidth; }

    [[nodiscard]] int16_t cellHeight() const { return m_cellHeight; }
    void setCellHeight(const int16_t cellHeight) { m_cellHeight = cellHeight; }

    [[nodiscard]] int16_t baseline() const { return m_baseline; }
    void setBaseline(const int16_t baseline) { m_baseline = baseline; }

    [[nodiscard]] int16_t kernStart() const { return m_kernStart; }
    void setKernStart(const int16_t kernStart) { m_kernStart = kernStart; }

    [[nodiscard]] int16_t layer() const { return m_layer; }
    void setLayer(const int16_t layer) { m_layer = layer; }

  private:
    char16_t m_char{};
    float m_startU{};
    float m_startV{};
    float m_endU{};
    float m_endV{};
    int8_t m_layer{};
    int16_t m_leftPadding{};
    int16_t m_advance{};
    int16_t m_rightPadding{};
    int16_t m_cellWidth{};
    int16_t m_cellHeight{};
    int16_t m_baseline{};
    int16_t m_kernStart{};
  };

  class FontInfo {
  public:
    FontInfo() = default;
    explicit FontInfo(athena::io::IStreamReader& in);
    explicit FontInfo(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;

    [[nodiscard]] bool unknown1() const { return m_unknown1; } // Bold?
    void setUnknown1(const bool unknown1) { m_unknown1 = unknown1; }
    [[nodiscard]] bool unknown2() const { return m_unknown2; } // Italic?
    void setUnknown2(const bool unknown2) { m_unknown2 = unknown2; }
    [[nodiscard]] int32_t unknown3() const { return m_unknown3; } // Weight?
    void setUnknown3(const int32_t unknown3) { m_unknown3 = unknown3; }
    [[nodiscard]] int32_t fontSize() const { return m_fontSize; }
    void setFontSize(const int32_t fontSize) { m_fontSize = fontSize; }
    [[nodiscard]] const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

  private:
    bool m_unknown1{};
    bool m_unknown2{};
    int32_t m_unknown3{};
    int32_t m_fontSize{};
    std::string m_name{};
  };

  class KernPair {
  public:
    KernPair() = default;
    explicit KernPair(athena::io::IStreamReader& in);
    explicit KernPair(const nlohmann::ordered_json& in);

    void PutTo(athena::io::IStreamWriter& out) const;
    void PutTo(nlohmann::ordered_json& out) const;

    [[nodiscard]] char16_t first() const { return m_first; }
    void setFirst(const char16_t first) { m_first = first; }
    [[nodiscard]] char16_t second() const { return m_second; }
    void setSecond(const char16_t second) { m_second = second; }

    [[nodiscard]] int32_t howMuch() const { return m_howMuch; }
    void setHowMuch(const int32_t howMuch) { m_howMuch = howMuch; }

  private:
    char16_t m_first;
    char16_t m_second;
    int32_t m_howMuch;
  };

  static constexpr auto kMagic = FOURCC('FONT');
  RasterFont(const char* ptr, std::size_t size);
  explicit RasterFont(const nlohmann::ordered_json& in);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;

  [[nodiscard]] uint32_t immediateChildCount() const override { return 1; }
  [[nodiscard]] std::optional<std::vector<std::shared_ptr<IResource>>> immediateChildren() const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata);

  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  uint32_t m_version{};
  uint32_t m_monoWidth{};
  uint32_t m_monoHeight{};
  uint32_t m_baseline{};
  uint32_t m_lineMargin{};
  FontInfo m_fontInfo{};
  AssetId32Big m_texture{};
  uint32_t m_mode;
  std::vector<Glyph> m_glyphs{};
  std::vector<KernPair> m_kernPairs{};
};
} // namespace axdl::primedep::MetroidPrime