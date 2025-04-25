#include "PrimeDep/Resources/AudioGroup.hpp"
#include "include/PrimeDep/ResourceFactory.hpp"
#include "PrimeDep/ResourceNameDatabase.hpp"
#include "PrimeDep/ResourcePool.hpp"
#include "PrimeDep/Resources/MapWorld.hpp"
#include "PrimeDep/Resources/MetroidWorld.hpp"
#include "PrimeDep/Resources/StringTable.hpp"
#include "PrimeDep/Resources/Texture.hpp"

#include <iostream>
#include <PrimeDep/Resources/PakFile.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std::string_view_literals;

#include <filesystem>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

std::filesystem::path executableDirectory() {
#ifdef _WIN32
  wchar_t path_buffer[MAX_PATH] = {0};
  GetModuleFileNameW(nullptr, path_buffer, MAX_PATH);
  return std::filesystem::path(path_buffer).parent_path();
#else
  char path_buffer[PATH_MAX] = {0};
  ssize_t count = readlink("/proc/self/exe", path_buffer, PATH_MAX);
  if (count == -1) {
    return std::filesystem::path();
  }
  return std::filesystem::path(std::string(path_buffer, (count > 0) ? count : 0)).parent_path();
#endif
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage " << argv[0] << " inputFolder outputFolder" << std::endl;
    return 1;
  }

  axdl::primedep::ResourceNameDatabase::instance().load((executableDirectory() / "ResourceDB.json").generic_string());
  // Initialize factory
  axdl::primedep::ResourceFactory32Big factory;
  axdl::primedep::RegisterFactory32Big<axdl::primedep::Texture>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::StringTable>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MapWorld>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::AudioGroup>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidWorld>(factory);

  // Spin up the pool
  auto* pool = axdl::primedep::ResourcePool32Big::instance();
  pool->setFactory(factory);

  // Gather paks
  std::filesystem::path inputFolder = argv[1];
  std::filesystem::path outputFolder = argv[2];

  std::shared_ptr<axdl::primedep::PakFile32Big> metroid1;
  for (const auto& entry : std::filesystem::directory_iterator(inputFolder)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    std::string extension = entry.path().extension();
    athena::utility::tolower(extension);

    if (extension != ".pak") {
      continue;
    }
    auto pak = axdl::primedep::PakFile32Big::load(entry.path().generic_string());
    auto repPath = relative(entry.path(), inputFolder);
    pak->writeMetadata((outputFolder / repPath).generic_string());
    pool->addSource(pak);
  }

#if 0
  if (!metroid1) {
    return 0;
  }

  for (const auto& desc : metroid1->resourceDescriptors()) {
    std::cout << std::format("\t{:8}\t0x{:08X}\t{} [{}]", desc.dataSize(), desc.assetId().id,
                             axdl::primedep::ResourceNameDatabase::instance().pathForAsset(
                                 axdl::primedep::ObjectTag32Big(desc.type(), desc.assetId())),
                             desc.isCompressed() ? "c" : "n")
              << std::endl;
  }
#endif
#if 0
  std::cout << "Gathering worlds..." << std::endl;
  auto worldTags = pool->tagsByType(axdl::primedep::FourCC("MLVL"sv));
  std::ranges::sort(worldTags.begin(), worldTags.end(), std::less<>());

  for (const auto& tag : worldTags) {
    const auto repPath = axdl::primedep::ResourceNameDatabase::instance().pathForAsset(tag);
    auto fileOut = std::filesystem::path(repPath);
    if (fileOut.generic_string().starts_with("$/")) {
      fileOut = fileOut.generic_string().substr(2, fileOut.generic_string().length() - 2);
    }

    std::cout << "Found " << repPath << std::endl;
    auto world = pool->resourceById(tag);
    if (world) {
      const auto outPath = (outputFolder / fileOut);
      if (!std::filesystem::exists(outPath)) {
        std::filesystem::create_directories(outPath.parent_path());
      }
      world->writeMetadata(outPath.generic_string(), repPath);
      world->writeUncooked(outPath.generic_string());
      // std::cout << world->metadata(repPath).dump(4) << std::endl;
      //  auto childTags = world->childTags();
      //  if (childTags) {
      //    for (const auto& t : *childTags) {
      //      const auto d = pool->resourceDescriptorById(t);
      //      const auto repPath = axdl::primedep::ResourceNameDatabase::instance().pathForAsset(t);
      //      auto fileOut = std::filesystem::path(repPath);
      //      if (fileOut.generic_string().starts_with("$/")) {
      //        fileOut = fileOut.generic_string().substr(2, fileOut.generic_string().length() - 2);
      //      }
      //      const auto outPath = (outputFolder / fileOut);
      //
      //
      //      std::cout << std::format("\t{:8}\t0x{:08X}\t{} [{}]", d.dataSize(), t.id.id, outPath.generic_string(),
      //                               d.isCompressed() ? "c" : "n")
      //                << std::endl;
      //    }
      //  }
    }
  }
#endif
#if 1
  auto stringTags = pool->tagsByType(axdl::primedep::kInvalidFourCC);
  for (const auto& tag : stringTags) {
    const auto repPath = axdl::primedep::ResourceNameDatabase::instance().pathForAsset(tag);
    auto fileOut = std::filesystem::path(repPath);
    if (fileOut.generic_string().starts_with("$/")) {
      fileOut = fileOut.generic_string().substr(2, fileOut.generic_string().length() - 2);
    }

    std::cout << "Found " << repPath << std::endl;
    auto string = pool->resourceById(tag);
    if (string) {
      const auto outPath = (outputFolder / fileOut);
      if (!std::filesystem::exists(outPath)) {
        std::filesystem::create_directories(outPath.parent_path());
      }
      string->writeMetadata(outPath.generic_string(), repPath);
      string->writeUncooked(outPath.generic_string());
    }
  }
#endif
  return 0;
}