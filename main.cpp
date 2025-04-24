#include "PrimeDep/Resources/AudioGroup.hpp"
#include "include/PrimeDep/ResourceFactory.hpp"
#include "PrimeDep/Resources/Texture.hpp"

#include <iostream>
#include <PrimeDep/Resources/PakFile.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std::string_view_literals;

struct ResourceDatabase {
  friend void from_json(const nlohmann::ordered_json& j, ResourceDatabase& db);
  std::map<axdl::primedep::AssetId32Big, std::string> assets; // ID -> rep path
};

void from_json(const nlohmann::ordered_json& j, ResourceDatabase& db) {
  for (const auto& j : j.items()) {
    std::string idStr;
    j.value().get_to(idStr);
    auto id = axdl::primedep::AssetId32Big(std::stol(idStr, nullptr, 16));
    db.assets[id] = j.key();
  }
}
int main() {
  const std::filesystem::path filename = "/home/antidote/MP1_0-00/files/Metroid8.pak";
  if (!std::filesystem::exists(filename)) {
    return 0;
  }
  
  const std::filesystem::path outputPath = "/home/antidote/MP1_0-00/rep";
  std::filesystem::create_directories(outputPath);

  axdl::primedep::ResourceFactory resourceFactory;
  resourceFactory.registerFactory(axdl::primedep::Texture::ResourceType(), axdl::primedep::Texture::create);
  resourceFactory.registerFactory(axdl::primedep::AudioGroup::ResourceType(), axdl::primedep::AudioGroup::create);

  std::ifstream dbFile("ResourceDB.json");

  ResourceDatabase db = ResourceDatabase(nlohmann::ordered_json::parse(dbFile));

  nlohmann::ordered_json js;

  if (auto file = axdl::primedep::PakFile32Big::load(filename.generic_string(), resourceFactory)) {
    js["PackageName"] = std::filesystem::path(file->path()).filename().replace_extension().generic_string();
    auto& namedResources = js["NamedResources"] = nlohmann::json::array();
    for (const auto& named : file->namedResources()) {
      if (!db.assets.contains(named.assetId())) {
        namedResources.push_back({{"Name", named.name()},
                                  {"Type", named.type().toString()},
                                  {"AssetID", std::format("{:08x}", named.assetId().id)}});
      } else {
        namedResources.push_back(
            {{"Type", named.type().toString()}, {"Name", named.name()}, {"File", db.assets[named.assetId()]}});
      }
    }

    std::vector<axdl::primedep::AssetId32Big> writtenIds;
    auto& resources = js["Resources"] = nlohmann::json::array();
    for (const auto& res : file->resourceDescriptors()) {
      if (std::ranges::contains(writtenIds, res.assetId())) {
        continue;
      }

      if (!db.assets.contains(res.assetId())) {
        std::string ext = res.type().toString();
        athena::utility::tolower(ext);
        const std::filesystem::path path =
            outputPath / "Uncategorized" / res.type().toString() / std::format("{:08X}.{}", res.assetId().id, ext);
        std::filesystem::create_directories(path.parent_path());
        resources.push_back({{"Type", res.type().toString()},
                             {"AssetID", std::format("{:08x}", res.assetId().id)},
                             {"Compress", res.isCompressed()}});
        file->writeUncompressedToFileSystem(path.generic_string(), res);
      } else {
        const auto path =
            std::filesystem::path(db.assets[res.assetId()].substr(2, db.assets[res.assetId()].length() - 2));

        std::cout << path << std::endl;
        std::filesystem::create_directories(outputPath / path.parent_path());
        resources.push_back(
            {{"Type", res.type().toString()}, {"File", db.assets[res.assetId()]}, {"Compress", res.isCompressed()}});
        file->writeUncompressedToFileSystem((outputPath / path).generic_string(), res);
      }
      writtenIds.emplace_back(res.assetId());
    }
  }
  std::ofstream test(outputPath / filename.filename().replace_extension(".json").generic_string());
  test << js.dump(4);
  return 0;
}