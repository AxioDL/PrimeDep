#include "PrimeDep/Resources/MetroidPrime/Hints.hpp"

#include "PrimeDep/Resources/MetroidPrime/AnimPOIData.hpp"

#include <athena/FileReader.hpp>
#include <athena/MemoryReader.hpp>

namespace axdl::primedep::MetroidPrime {

Hints::HintLocation::HintLocation(athena::io::IStreamReader& in)
: m_worldId(in, FOURCC('MLVL'))
, m_areaId(in, FOURCC('MREA'))
, m_roomId(in.readUint32Big())
, m_stringId(in, FOURCC('STRG')) {}

Hints::HintLocation::HintLocation(const nlohmann::ordered_json& in)
: m_worldId(in.value("World", nlohmann::ordered_json()), FOURCC('MLVL'))
, m_areaId(in.value("Area", nlohmann::ordered_json()), FOURCC('MREA'))
, m_roomId(in.value("World", -1))
, m_stringId(in.value("String", nlohmann::ordered_json()), FOURCC('STRG')) {}

void Hints::HintLocation::PutTo(athena::io::IStreamWriter& out) const {
  m_worldId.PutTo(out);
  m_areaId.PutTo(out);
  out.writeUint32Big(m_roomId);
  m_stringId.PutTo(out);
}
void Hints::HintLocation::PutTo(nlohmann::ordered_json& out) const {
  m_worldId.PutTo(out["World"], FOURCC('MLVL'));
  m_areaId.PutTo(out["Area"], FOURCC('MREA'));
  out["Room"] = m_roomId;
  m_stringId.PutTo(out["String"], FOURCC('STRG'));
}

Hints::Hint::Hint(athena::io::IStreamReader& in)
: m_name(in.readString())
, m_immediateTime(in.readFloatBig())
, m_normalTime(in.readFloatBig())
, m_stringId(in, FOURCC('STRG'))
, m_textTime(in.readInt32Big()) {
  uint32_t count = in.readUint32Big();
  while (count--) {
    m_locations.emplace_back(in);
  }
}

Hints::Hint::Hint(const nlohmann::ordered_json& in)
: m_name(in.value("Name", ""))
, m_immediateTime(in.value("ImmediateTime", 0.f))
, m_normalTime(in.value("NormalTime", 0.f))
, m_stringId(in.value("String", nlohmann::ordered_json()), FOURCC('STRG'))
, m_textTime(in.value("TextTime", 0)) {

  if (in.contains("Locations")) {
    for (const auto& locations = in["Locations"]; const auto& location : locations) {
      m_locations.emplace_back(location);
    }
  }
}

void Hints::Hint::PutTo(athena::io::IStreamWriter& out) const {
  out.writeString(m_name);
  out.writeFloatBig(m_immediateTime);
  out.writeFloatBig(m_normalTime);
  m_stringId.PutTo(out);
  out.writeInt32Big(m_textTime);
  out.writeUint32Big(m_locations.size());
  for (const auto& location : m_locations) {
    location.PutTo(out);
  }
}

void Hints::Hint::PutTo(nlohmann::ordered_json& out) const {
  out["Name"] = m_name;
  out["ImmediateTime"] = m_immediateTime;
  out["NormalTime"] = m_normalTime;
  m_stringId.PutTo(out["String"], FOURCC('STRG'));
  out["TextTime"] = m_textTime;
  auto& locations = out["Locations"];
  for (const auto& location : m_locations) {
    location.PutTo(locations.emplace_back());
  }
}

Hints::Hints(const char* ptr, std::size_t size) {
  athena::io::MemoryReader in(ptr, size);
  const auto magic = in.readUint32Big();
  if (magic != kMagic) {
    printf("Unexpected Hint magic, got %08X, expected %08X\n", magic, kMagic);
    return;
  }

  const auto version = in.readUint32Big();
  if (version != kVersion) {
    printf("Unexpected Hint version, got %i, expected %i\n", version, kVersion);
    return;
  }

  auto count = in.readUint32Big();

  while (count--) {
    m_hints.emplace_back(in);
  }
}

Hints::Hints(const nlohmann::ordered_json& in) {
  if (in.contains("Hints")) {
    const auto hints = in["Hints"];
    for (const auto& hint : hints) {
      m_hints.emplace_back(hint);
    }
  }
}

bool Hints::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());
  out.writeUint32Big(kMagic);
  out.writeUint32Big(kVersion);

  out.writeUint32Big(m_hints.size());
  for (const auto& hint : m_hints) {
    hint.PutTo(out);
  }

  return !out.hasError();
}

bool Hints::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);

  nlohmann::ordered_json j;
  auto& hints = j["Hints"];
  hints = nlohmann::ordered_json::array();
  for (const auto& hint : m_hints) {
    hint.PutTo(hints.emplace_back());
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = j.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> Hints::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<Hints>(ptr, size);
}

std::shared_ptr<IResource> Hints::ingest([[maybe_unused]] const nlohmann::ordered_json& metadata,
                                         const std::string_view path) {
  athena::io::FileReader in(path);
  auto js = nlohmann::ordered_json::parse(in.readString());
  return std::make_shared<Hints>(js);
}
} // namespace axdl::primedep::MetroidPrime