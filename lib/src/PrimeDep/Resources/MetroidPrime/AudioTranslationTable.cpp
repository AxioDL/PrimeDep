#include "PrimeDep/Resources/MetroidPrime/AudioTranslationTable.hpp"

#include <athena/FileReader.hpp>
#include <athena/MemoryReader.hpp>

namespace axdl::primedep::MetroidPrime {
AudioTranslationTable::AudioTranslationTable(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  auto sfxCount = in.readUint32Big();

  while (sfxCount--) {
    m_sfxIds.push_back(in.readUint16Big());
  }
}

AudioTranslationTable::AudioTranslationTable(const nlohmann::ordered_json& in) {
  const auto& ids = in["SoundEffectIDs"];
  for (const auto& id : ids) {
    m_sfxIds.emplace_back(id);
  }
}

bool AudioTranslationTable::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());

  out.writeUint32Big(m_sfxIds.size());
  for (const auto& sfxId : m_sfxIds) {
    out.writeUint16Big(sfxId);
  }

  return !out.hasError();
}

bool AudioTranslationTable::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json j;
  auto& sfxIds = j["SoundEffectIDs"];
  for (const auto sfx : m_sfxIds) {
    sfxIds.emplace_back(sfx);
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = j.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> AudioTranslationTable::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<AudioTranslationTable>(ptr, size);
}

bool AudioTranslationTable::canIngest(const nlohmann::ordered_json& metadata) {
  return metadata["ResourceType"] == ResourceType().toString();
}
std::shared_ptr<IResource> AudioTranslationTable::ingest(const nlohmann::ordered_json& metadata,
                                                         std::string_view path) {
  const auto p = GetRawPath(path);
  athena::io::FileReader in(p.generic_string());
  auto js = nlohmann::ordered_json::parse(in.readString());
  if (!js.contains("SoundEffectIDs")) {
    return nullptr;
  }

  return std::make_shared<AudioTranslationTable>(js);
}

} // namespace axdl::primedep::MetroidPrime