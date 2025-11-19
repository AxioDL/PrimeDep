#include "PrimeDep/Resources/MetroidPrime/RasterFont.hpp"

#include "PrimeDep/ResourceFactory.hpp"
#include "PrimeDep/ResourcePool.hpp"
#include "tiny_gltf.h"

#include <athena/FileReader.hpp>
#include <athena/MemoryReader.hpp>

namespace axdl::primedep ::MetroidPrime {
RasterFont::Glyph::Glyph(athena::io::IStreamReader& in, uint32_t version)
: m_char(in.readUint16Big())
, m_startU(in.readFloatBig())
, m_startV(in.readFloatBig())
, m_endU(in.readFloatBig())
, m_endV(in.readFloatBig())
, m_layer(static_cast<int16_t>(version >= 4 ? in.readByte() : 0))
, m_leftPadding(static_cast<int16_t>(version < 4 ? in.readInt32Big() : in.readByte()))
, m_advance(static_cast<int16_t>(version < 4 ? in.readInt32Big() : in.readByte()))
, m_rightPadding(static_cast<int16_t>(version < 4 ? in.readInt32Big() : in.readByte()))
, m_cellWidth(static_cast<int16_t>(version < 4 ? in.readInt32Big() : in.readByte()))
, m_cellHeight(static_cast<int16_t>(version < 4 ? in.readInt32Big() : in.readByte()))
, m_baseline(static_cast<int16_t>(version < 4 ? in.readInt32Big() : in.readByte()))
, m_kernStart(static_cast<int16_t>(version < 4 ? in.readInt32Big() : in.readInt16Big())) {}

RasterFont::Glyph::Glyph(const nlohmann::ordered_json& in) {
  if (const auto chr = convertStringToUTF16(in.value("Character", "")); !chr.empty()) {
    m_char = chr.at(0);
  }
  m_startU = in.value("StartU", 0.f);
  m_startV = in.value("StartV", 0.f);
  m_endU = in.value("EndU", 0.f);
  m_endV = in.value("EndV", 0.f);
  m_layer = static_cast<int8_t>(in.value("Layer", 0));
  m_leftPadding = static_cast<int16_t>(in.value("LeftPadding", 0));
  m_advance = static_cast<int16_t>(in.value("Advance", 0));
  m_rightPadding = static_cast<int16_t>(in.value("RightPadding", 0));
  m_cellWidth = static_cast<int16_t>(in.value("CellWidth", 0));
  m_cellHeight = static_cast<int16_t>(in.value("CellHeight", 0));
  m_baseline = static_cast<int16_t>(in.value("Baseline", 0));
  m_kernStart = static_cast<int16_t>(in.value("KernStart", 0));
}

void RasterFont::Glyph::PutTo(athena::io::IStreamWriter& out, const uint32_t version) const {
  out.writeUint16Big(m_char);
  out.writeFloatBig(m_startU);
  out.writeFloatBig(m_startV);
  out.writeFloatBig(m_endU);
  out.writeFloatBig(m_endV);
  if (version >= 4) {
    out.writeByte(m_layer);
    out.writeByte(static_cast<int8_t>(m_leftPadding));
    out.writeByte(static_cast<int8_t>(m_advance));
    out.writeByte(static_cast<int8_t>(m_rightPadding));
    out.writeByte(static_cast<int8_t>(m_cellWidth));
    out.writeByte(static_cast<int8_t>(m_cellHeight));
    out.writeByte(static_cast<int8_t>(m_baseline));
    out.writeInt16Big(m_kernStart);
  } else {
    out.writeInt32Big(m_leftPadding);
    out.writeInt32Big(m_advance);
    out.writeInt32Big(m_rightPadding);
    out.writeInt32Big(m_cellWidth);
    out.writeInt32Big(m_cellHeight);
    out.writeInt32Big(m_baseline);
    out.writeInt32Big(m_kernStart);
  }
}

void RasterFont::Glyph::PutTo(nlohmann::ordered_json& out, const uint32_t version) const {
  out["Character"] = convertUTF16ToString(std::u16string(1, m_char));
  out["StartU"] = m_startU;
  out["StartV"] = m_startV;
  out["EndU"] = m_endU;
  out["EndV"] = m_endV;
  if (version >= 4) {
    out["Layer"] = m_layer;
  }
  out["LeftPadding"] = m_leftPadding;
  out["Advance"] = m_advance;
  out["RightPadding"] = m_rightPadding;
  out["CellWidth"] = m_cellWidth;
  out["CellHeight"] = m_cellHeight;
  out["Baseline"] = m_baseline;
  out["KernStart"] = m_kernStart;
}

RasterFont::FontInfo::FontInfo(athena::io::IStreamReader& in)
: m_unknown1(in.readBool())
, m_unknown2(in.readBool())
, m_unknown3(in.readInt32Big())
, m_fontSize(in.readInt32Big())
, m_name(in.readString()) {}

RasterFont::FontInfo::FontInfo(const nlohmann::ordered_json& in) {
  m_unknown1 = in.value("Unknown3", m_unknown3);
  m_unknown2 = in.value("Unknown3", m_unknown3);
  m_unknown3 = in.value("Unknown3", m_unknown3);
  m_fontSize = in.value("FontSize", 0);
  m_name = in.value("Name", "");
}

void RasterFont::FontInfo::PutTo(athena::io::IStreamWriter& out) const {
  out.writeBool(m_unknown1);
  out.writeBool(m_unknown2);
  out.writeUint32Big(m_unknown3);
  out.writeUint32Big(m_fontSize);
  out.writeString(m_name);
}

void RasterFont::FontInfo::PutTo(nlohmann::ordered_json& out) const {
  out["Unknown1"] = m_unknown1;
  out["Unknown2"] = m_unknown2;
  out["Unknown3"] = m_unknown3;
  out["FontSize"] = m_fontSize;
  out["Name"] = m_name;
}

RasterFont::KernPair::KernPair(athena::io::IStreamReader& in)
: m_first(in.readUint16Big()), m_second(in.readUint16Big()), m_howMuch(in.readInt32Big()) {}

RasterFont::KernPair::KernPair(const nlohmann::ordered_json& in) {
  const auto first = convertStringToUTF16(in.value("First", ""));
  const auto second = convertStringToUTF16(in.value("Second", ""));
  if (!first.empty()) {
    m_first = first.at(0);
  }
  if (!second.empty()) {
    m_second = second.at(0);
  }
  m_howMuch = in.value("HowMuch", 0);
}

void RasterFont::KernPair::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint16Big(m_first);
  out.writeUint16Big(m_second);
  out.writeInt32Big(m_howMuch);
}

void RasterFont::KernPair::PutTo(nlohmann::ordered_json& out) const {
  out["First"] = convertUTF16ToString(std::u16string(1, m_first));
  out["Second"] = convertUTF16ToString(std::u16string(1, m_second));
  out["HowMuch"] = m_howMuch;
}

RasterFont::RasterFont(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  const auto magic = FourCC(in);
  assert(magic == kMagic);
  m_version = in.readUint32Big();

  m_monoHeight = in.readUint32Big();
  m_monoWidth = in.readUint32Big();

  if (m_version >= 1) {
    m_baseline = in.readUint32Big();
  }

  if (m_version >= 2) {
    m_lineMargin = in.readUint32Big();
  }

  m_fontInfo = FontInfo(in);

  m_texture = AssetId32Big(in, FOURCC('TXTR'));
  m_mode = in.readUint32Big();
  uint32_t glyphCount = in.readUint32Big();

  m_glyphs.reserve(glyphCount);
  while (glyphCount--) {
    m_glyphs.emplace_back(in, m_version);
  }

  uint32_t kernCount = in.readUint32Big();
  m_kernPairs.reserve(kernCount);

  while (kernCount--) {
    m_kernPairs.emplace_back(in);
  }
}

RasterFont::RasterFont(const nlohmann::ordered_json& in)
: m_version(in.value("Version", 4)), m_monoWidth(in.value("MonoWidth", 0)), m_monoHeight(in.value("MonoHeight", 0)) {
  if (m_version >= 1) {
    m_baseline = in.value("Baseline", 0);
  }
  if (m_version >= 2) {
    m_lineMargin = in.value("LineMargin", 0);
  }
  m_fontInfo = FontInfo(in.value("FontInfo", nlohmann::ordered_json()));
  m_texture = AssetId32Big(in, FOURCC('TXTR'));
  m_mode = in.value("Mode", 0);
  for (const auto& glyphs = in.value("Glyphs", nlohmann::ordered_json()); const auto& glyph : glyphs) {
    m_glyphs.emplace_back(glyph);
  }

  for (const auto& kernPairs = in.value("KernPairs", nlohmann::ordered_json()); const auto& kernPair : kernPairs) {
    m_kernPairs.emplace_back(kernPair);
  }
}

std::optional<std::vector<std::shared_ptr<IResource>>> RasterFont::children() const {
  std::vector<std::shared_ptr<IResource>> children;
  /* First try by id */
  auto res = ResourcePool32Big::instance()->resourceById(ObjectTag32Big(FOURCC('TXTR'), m_texture));

  if (!res) {
    /* If that fails try by rep path */
    res = ResourcePool32Big::instance()->ingestResourceByRepPath(m_texture.repPath(), FOURCC('TXTR'));
  }

  /* Even if both fail we still push the result, we check for null during the package/cook phase */
  children.emplace_back(res);
  return children;
}

bool RasterFont::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());
  kMagic.PutTo(out);
  out.writeUint32Big(m_version);
  out.writeUint32Big(m_monoWidth);
  out.writeUint32Big(m_monoHeight);
  if (m_version >= 1) {
    out.writeUint32Big(m_baseline);
  }

  if (m_version >= 2) {
    out.writeUint32Big(m_lineMargin);
  }

  m_fontInfo.PutTo(out);
  m_texture.PutTo(out);
  out.writeUint32Big(m_mode);
  out.writeUint32Big(m_glyphs.size());
  for (const auto& glyph : m_glyphs) {
    glyph.PutTo(out, m_version);
  }
  out.writeUint32Big(m_kernPairs.size());
  for (const auto& pair : m_kernPairs) {
    pair.PutTo(out);
  }

  return !out.hasError();
}

bool RasterFont::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json out;
  out["Version"] = m_version;
  out["MonoWidth"] = m_monoWidth;
  out["MonoHeight"] = m_monoHeight;
  if (m_version >= 1) {
    out["Baseline"] = m_baseline;
  }
  if (m_version >= 2) {
    out["LineMargin"] = m_lineMargin;
  }

  m_fontInfo.PutTo(out["FontInfo"]);
  m_texture.PutTo(out["Texture"], FOURCC('TXTR'));
  out["Mode"] = m_mode;

  auto& glyphs = out["Glyphs"];
  for (const auto& glyph : m_glyphs) {
    glyph.PutTo(glyphs.emplace_back(), m_version);
  }

  auto& kernPairs = out["KernPairs"];
  for (const auto& pair : m_kernPairs) {
    pair.PutTo(kernPairs.emplace_back());
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = out.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> RasterFont::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<RasterFont>(ptr, size);
}

bool RasterFont::canIngest(const nlohmann::ordered_json& metadata) {
  return metadata["ResourceType"] == ResourceType().toString();
}

std::shared_ptr<IResource> RasterFont::ingest([[maybe_unused]] const nlohmann::ordered_json& metadata,
                                              const std::string_view path) {
  const auto p = GetRawPath(path);
  athena::io::FileReader reader(p.generic_string());
  nlohmann::ordered_json in = nlohmann::ordered_json::parse(reader.readString());
  return std::make_shared<RasterFont>(in);
}
} // namespace axdl::primedep::MetroidPrime