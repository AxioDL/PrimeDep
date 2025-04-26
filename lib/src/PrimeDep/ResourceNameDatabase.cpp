#include "PrimeDep/ResourceNameDatabase.hpp"

#include "athena/FileReader.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep {
std::string ResourceNameDatabase::pathForAsset(const ObjectTag32Big& tag) {
  if (!assets.contains(tag.id)) {
    std::string extension = tag.type.toString();
    athena::utility::tolower(extension);
    return std::format("$/Uncategorized/{}/{:08X}.{}", tag.type.toString(), tag.id.id, extension);
  }

  return assets[tag.id];
}

void ResourceNameDatabase::load(const std::string_view filename) {
  athena::io::FileReader reader(filename);
  std::string contents = reader.readString();
  const auto& json = nlohmann::ordered_json::parse(contents);
  for (const auto& j : json.items()) {
    std::string idStr;
    j.value().get_to(idStr);
    auto id = AssetId32Big(std::stoll(idStr, nullptr, 16));
    assets[id] = j.key();
  }
}
} // namespace axdl::primedep