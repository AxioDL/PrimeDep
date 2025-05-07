#include "PrimeDep/Resources/MetroidPrime/StringTable.hpp"

#include "PrimeDep/Utils.hpp"
#include "athena/MemoryWriter.hpp"
#include "magic_enum/magic_enum.hpp"

#include <athena/MemoryReader.hpp>

namespace axdl::primedep ::MetroidPrime {
namespace {
constexpr std::array kLanguages{
    FOURCC('ENGL'), FOURCC('FREN'), FOURCC('GERM'), FOURCC('SPAN'), FOURCC('ITAL'), FOURCC('DUTC'), FOURCC('JAPN'),
};
} // Anonymous namespace

StringTable::StringTable(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  const auto magic = in.readUint32Big();
  assert(magic == kMagicNum && "Unexpected magic number");
  m_version = static_cast<EVersion>(in.readUint32Big());
  assert(m_version == EVersion::MetroidPrime1 && "Unsupported version");

  auto languageCount = in.readUint32Big();
  const auto stringCount = in.readUint32Big();

  std::vector<std::pair<FourCC, uint32_t>> languageOffsets;
  languageOffsets.reserve(languageCount);
  while (languageCount--) {
    const auto fcc = FourCC(in);
    const auto offset = in.readUint32Big();
    languageOffsets.emplace_back(fcc, offset);
  }

  const auto dataStart = in.position();
  for (auto& languageOffset : languageOffsets) {
    in.seek(dataStart + languageOffset.second, athena::SeekOrigin::Begin);
    StringPool& pool = m_languages[languageOffset.first];
    pool.m_strings.resize(stringCount);
    in.readUint32Big();
    std::vector<uint32_t> stringOffsets;
    const auto poolStart = in.position();
    for (int j = 0; j < stringCount; ++j) {
      stringOffsets.emplace_back(in.readUint32Big());
    }
    for (int j = 0; j < stringCount; j++) {
      in.seek(poolStart + stringOffsets[j], athena::SeekOrigin::Begin);
      pool.m_strings[j] = convertUTF16ToString(in.readU16StringBig());
    }
  }
}

nlohmann::ordered_json StringTable::metadata(std::string_view repPath) const {
  auto ret = ITypedResource::metadata(repPath);
  ret["Version"] = magic_enum::enum_name(m_version);
  for (const auto& fcc : m_languages | std::views::keys) {
    ret["Languages"].push_back(fcc.toString());
  }
  return ret;
}

std::string StringTable::string(const uint32_t idx, const FourCC& lang) const {
  if (!m_languages.contains(lang)) {
    return {};
  }

  const auto& pool = m_languages.at(lang);
  if (pool.m_strings.size() <= idx) {
    return {};
  }

  return pool.m_strings[idx];
}

bool StringTable::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);

  athena::io::FileWriter out(p.generic_string());
  out.writeUint32Big(kMagicNum);
  out.writeUint32Big(static_cast<uint32_t>(m_version));
  out.writeUint32Big(static_cast<uint32_t>(m_languages.size()));
  const auto& english = m_languages.at(FOURCC('ENGL'));
  out.writeUint32Big(english.m_strings.size());

  auto fccStart = out.position();
  // We don't know where the languages will end up right now, so just write 0s
  for (const auto& fcc : m_languages | std::views::keys) {
    out.writeUint32Big(0);
    out.writeUint32Big(0);
  }

  std::map<FourCC, uint32_t> languageOffsets;

  const auto dataStart = out.position();
  for (const auto fcc : kLanguages) {
    if (!m_languages.contains(fcc)) {
      continue;
    }
    const auto& pool = m_languages.at(fcc);
    // Store the offset, we want a position relative to the start of the language string pools so account for that
    languageOffsets[fcc] = out.position() - dataStart;
    athena::io::MemoryCopyWriter stringPool;
    stringPool.writeUint32Big(0);
    // Write 0s for now since we don't know where the strings are yet
    for (int i = 0; i < english.m_strings.size(); ++i) {
      stringPool.writeUint32Big(0);
    }
    std::vector<uint32_t> stringOffsets;
    stringOffsets.reserve(english.m_strings.size());
    for (int i = 0; i < english.m_strings.size(); ++i) {
      // String positions are relative to the pool's start after the stored buffer size
      stringOffsets.emplace_back(stringPool.position() - sizeof(uint32_t));
      // If we don't have a translation for this string, use the english one
      if (i < pool.m_strings.size()) {
        stringPool.writeU16StringBig(convertStringToUTF16(pool.m_strings[i]));
      } else {
        stringPool.writeU16StringBig(convertStringToUTF16(english.m_strings[i]));
      }
    }
    // We now have the metadata we need to store, lets do that now
    stringPool.seek(0, athena::SeekOrigin::Begin);
    // The string pool's buffer size only includes the string offsets and strings, so remove the 4 bytes for the buffer
    // length
    stringPool.writeUint32Big(stringPool.length() - sizeof(uint32_t));

    // Now store the string offsets we gathered in the above loop
    for (int i = 0; i < english.m_strings.size(); ++i) {
      stringPool.writeUint32Big(stringOffsets[i]);
    }

    // Finally flush the string pool to the file!
    out.writeBytes(stringPool.data(), stringPool.length());
  }

  out.seek(fccStart, athena::SeekOrigin::Begin);
  for (const auto& fcc : kLanguages) {
    if (!languageOffsets.contains(fcc)) {
      continue;
    }
    out.writeBytes(fcc.toString().c_str(), 4);
    out.writeUint32Big(languageOffsets[fcc]);
  }

  return !out.hasError();
}

bool StringTable::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json json;
  json["Version"] = magic_enum::enum_name(m_version);
  for (const auto& languageFcc : kLanguages) {
    if (!m_languages.contains(languageFcc)) {
      continue;
    }

    const auto& language = m_languages.at(languageFcc);
    auto& lang = json["Languages"].emplace_back();
    lang["Region"] = languageFcc.toString();
    auto& strings = lang["Strings"];
    for (const auto& m_string : language.m_strings) {
      strings.push_back(m_string);
    }
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = json.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> StringTable::loadCooked(const char* ptr, const std::size_t size) {
  return std::make_shared<StringTable>(ptr, size);
}
bool StringTable::canIngest(const nlohmann::ordered_json& metadata) {
  return metadata["ResourceType"] == ResourceType().toString();
}
std::shared_ptr<IResource> StringTable::ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
  return nullptr;
}

} // namespace axdl::primedep::MetroidPrime