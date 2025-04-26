#include "PrimeDep/Resources/StringTable.hpp"

#include "magic_enum/magic_enum.hpp"
#include "PrimeDep/Utils.hpp"

#include <athena/MemoryReader.hpp>

namespace axdl::primedep {
namespace {
constexpr std::array languages{
    FOURCC('ENGL'), FOURCC('FREN'), FOURCC('GERM'), FOURCC('SPAN'), FOURCC('ITAL'), FOURCC('DUTC'), FOURCC('JAPN'),
};
} // Anonymous namespace

StringTable::StringTable(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader in(ptr, size, true);
  uint32_t magic = in.readUint32Big();
  assert(magic == kMagicNum && "Unexpected magic number");
  m_version = static_cast<EVersion>(in.readUint32Big());
  assert(m_version == EVersion::MetroidPrime1 && "Unsupported version");

  uint32_t languageCount = in.readUint32Big();
  const uint32_t stringCount = in.readUint32Big();

  std::vector<std::pair<FourCC, uint32_t>> languageOffsets;
  languageOffsets.reserve(languageCount);
  while (languageCount--) {
    const auto fcc = FourCC(in);
    const auto offset = in.readUint32Big();
    languageOffsets.emplace_back(fcc, offset);
  }

  const auto dataStart = in.position();
  for (int i = 0; i < languageOffsets.size(); i++) {
    in.seek(dataStart + languageOffsets[i].second, athena::SeekOrigin::Begin);
    StringPool& pool = m_languages[languageOffsets[i].first];
    pool.m_bufLen = in.readUint32Big();
    pool.m_strings = std::make_unique<char16_t[]>(pool.m_bufLen / sizeof(char16_t));
    for (int j = 0; j < stringCount; ++j) {
      // Account for the string lookup table and string count
      pool.m_stringOffsets.emplace_back((in.readUint32Big() - (sizeof(uint32_t) * stringCount)) / sizeof(uint16_t));
    }
    in.readUBytesToBuf(pool.m_strings.get(), pool.m_bufLen);
    for (int j = 0; j < pool.m_bufLen / sizeof(char16_t); j++) {
      pool.m_strings[j] = SBig(static_cast<uint16_t>(pool.m_strings[j]));
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

std::u16string StringTable::string(const uint32_t idx, const FourCC& lang) const {
  if (!m_languages.contains(lang)) {
    return {};
  }

  const auto& pool = m_languages.at(lang);
  if (pool.m_stringOffsets.size() <= idx) {
    return {};
  }

  return pool.m_strings.get() + pool.m_stringOffsets[idx];
}

std::shared_ptr<IResource> StringTable::loadCooked(const char* ptr, std::size_t size,
                                                   const ResourceDescriptor32Big& desc) {
  return std::make_shared<StringTable>(ptr, size, desc);
}

bool StringTable::writeUncooked(const std::string_view path) const {
  std::filesystem::path p(path);
  while (p.has_extension()) {
    p.replace_extension();
  }
  p.replace_extension(".strg");

  nlohmann::ordered_json json;
  json["Version"] = magic_enum::enum_name(m_version);
  for (const auto& language : m_languages) {
    auto& lang = json["Languages"].emplace_back();
    lang["Region"] = language.first.toString();
    auto& strings = lang["Strings"];
    for (int i = 0; i < language.second.m_stringOffsets.size(); ++i) {
      strings.push_back(convertUTF16ToString(string(i, language.first)));
    }
  }

  athena::io::FileWriter writer(p.generic_string());
  std::string js = json.dump(4);
  writer.writeString(js, js.length());
  return !writer.hasError();
}
} // namespace axdl::primedep