#include "PrimeDep/Resources/AudioGroup.hpp"
#include "PrimeDep/Resources/ResourceFactory.hpp"
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
  axdl::primedep::ResourceFactory resourceFactory;
  resourceFactory.registerFactory(axdl::primedep::Texture::ResourceType(), axdl::primedep::Texture::create);
  resourceFactory.registerFactory(axdl::primedep::AudioGroup::ResourceType(), axdl::primedep::AudioGroup::create);

  std::ifstream dbFile("ResourceDB.json");

  ResourceDatabase db = ResourceDatabase(nlohmann::ordered_json::parse(dbFile));

  nlohmann::ordered_json js;

  if (auto file = axdl::primedep::PakFile32Big::load("/home/antidote/MP1_0-00/files/Metroid4.pak", resourceFactory)) {
    js["PackageName"] = std::filesystem::path(file->path()).filename().replace_extension().generic_string();
    auto& namedResources = js["NamedResources"] = nlohmann::json::array();
    for (const auto& named : file->namedResources()) {
      const uint32_t type = athena::utility::swap32(named.type());
      const std::string typeStr = std::format("{:.4}", reinterpret_cast<const char*>(&type));
      if (!db.assets.contains(named.assetId())) {
        namedResources.push_back(
            {{"Name", named.name()}, {"Type", typeStr}, {"AssetID", std::format("{:08x}", named.assetId().id)}});
      } else {
        namedResources.push_back({{"Type", typeStr}, {"Name", named.name()}, {"File", db.assets[named.assetId()]}});
      }
    }

    std::vector<axdl::primedep::AssetId32Big> writtenIds;
    auto& resources = js["Resources"] = nlohmann::json::array();
    for (const auto& res : file->resourceDescriptors()) {
      if (std::ranges::contains(writtenIds, res.assetId())) {
        continue;
      }

      const uint32_t type = athena::utility::swap32(res.type());
      const std::string typeStr = std::format("{:.4}", reinterpret_cast<const char*>(&type));
      if (!db.assets.contains(res.assetId())) {
        resources.push_back({{"Type", typeStr},
                             {"AssetID", std::format("{:08x}", res.assetId().id)},
                             {"Compress", res.isCompressed()}});
      } else {
        resources.push_back({{"Type", typeStr}, {"File", db.assets[res.assetId()]}, {"Compress", res.isCompressed()}});
      }
      writtenIds.emplace_back(res.assetId());
    }
  }
  std::ofstream test("Metroid4.json");
  test << js.dump(4);
  return 0;
}