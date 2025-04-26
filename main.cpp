#include "PrimeDep/Resources/AudioGroup.hpp"
#include "include/PrimeDep/ResourceFactory.hpp"
#include "PrimeDep/ResourceNameDatabase.hpp"
#include "PrimeDep/ResourcePool.hpp"
#include "PrimeDep/Resources/AnimPOIData.hpp"
#include "PrimeDep/Resources/AnimSource.hpp"
#include "PrimeDep/Resources/CharLayoutInfo.hpp"
#include "PrimeDep/Resources/CollisionResponseData.hpp"
#include "PrimeDep/Resources/GuiFrame.hpp"
#include "PrimeDep/Resources/MapWorld.hpp"
#include "PrimeDep/Resources/MetroidWorld.hpp"
#include "PrimeDep/Resources/Model.hpp"
#include "PrimeDep/Resources/Particle.hpp"
#include "PrimeDep/Resources/ParticleElectric.hpp"
#include "PrimeDep/Resources/ParticleSwoosh.hpp"
#include "PrimeDep/Resources/ProjectileWeapon.hpp"
#include "PrimeDep/Resources/RasterFont.hpp"
#include "PrimeDep/Resources/ScannableObjectInfo.hpp"
#include "PrimeDep/Resources/StringTable.hpp"
#include "PrimeDep/Resources/SkinRules.hpp"
#include "PrimeDep/Resources/Texture.hpp"

#include <iostream>
#include <PrimeDep/Resources/PakFile.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <set>

using namespace std::string_view_literals;

#include <filesystem>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
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
  const auto count = readlink("/proc/self/exe", path_buffer, PATH_MAX);
  if (count == -1) {
    return {};
  }
  return std::filesystem::path(std::string(path_buffer, (count > 0) ? count : 0)).parent_path();
#endif
}

void addFactories(axdl::primedep::ResourceFactory32Big& factory) {
  axdl::primedep::RegisterFactory32Big<axdl::primedep::StringTable>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::Model>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::Texture>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::SkinRules>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::AnimSource>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::CharLayoutInfo>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::CollisionResponseData>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::ParticleSwoosh>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::Particle>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::ParticleElectric>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::ProjectileWeapon>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::GuiFrame>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::RasterFont>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::ScannableObjectInfo>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::AnimPOIData>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::AiFiniteStateMachine>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::AudioGroup>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::CollidableOBBTreeGroup>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::DecalData>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::AudioTranslationTable>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::PathFindArea>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MapWorld>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidArea(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::MapArea>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::MapUniverse>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::MidiData>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::SaveWorld>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidWorld>(factory);
}

int main(int argc, char** argv) {
  const auto locale = std::setlocale(LC_ALL, "");
  std::cout << locale << std::endl;
  std::cout.imbue(std::locale(locale));
#if _WIN32
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  // Get the current console mode
  DWORD dwMode = 0;
  if (!GetConsoleMode(hConsole, &dwMode)) {
    std::cerr << "Error hStdout console mode: " << GetLastError() << std::endl;
    return 1;
  }

  // Enable virtual terminal processing
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(hStdout, dwMode)) {
    std::cerr << "Error setting console mode: " << GetLastError() << std::endl;
    return 1;
  }
  SetConsoleMode(hStdout, ENABLE_VIRTUAL_TERMINAL_PROCESSING)
#endif
      if (argc != 3) {
    std::cout << "Usage " << argv[0] << " inputFolder outputFolder" << std::endl;
    return 1;
  }
  axdl::primedep::ResourceNameDatabase::instance().load((executableDirectory() / "ResourceDB.json").generic_string());
  // Initialize factory
  axdl::primedep::ResourceFactory32Big factory;
  addFactories(factory);

  // Spin up the pool
  auto* pool = axdl::primedep::ResourcePool32Big::instance();
  pool->setFactory(factory);

  // Gather paks
  std::filesystem::path inputFolder = argv[1];
  std::filesystem::path outputFolder = argv[2];

  // TODO: Gather this information from the binary
  nlohmann::ordered_json manifest;
  manifest["Game"] = "Metroid Prime 1";
  manifest["GameVersion"] = "v1.088";

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
    if (!exists(outputFolder)) {
      create_directories(outputFolder);
    }
    pak->writeMetadata((outputFolder / repPath).generic_string());
    pool->addSource(pak);
    manifest["Paks"].push_back("$" / repPath.replace_extension(".prj"));
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
  // std::ranges::sort(stringTags.begin(), stringTags.end(), std::less<>());
  auto uniqueTags = std::set(stringTags.begin(), stringTags.end());

  printf("\n\n");
  for (int i = 0; const auto& tag : uniqueTags) {
    const auto repPath = axdl::primedep::ResourceNameDatabase::instance().pathForAsset(tag);
    auto fileOut = std::filesystem::path(repPath);
    if (fileOut.generic_string().starts_with("$/")) {
      fileOut = fileOut.generic_string().substr(2, fileOut.generic_string().length() - 2);
    }

    std::cout << "\033[2;A";
    std::cout << "\033[0;KProgress: " << (i + 1) << " of " << uniqueTags.size()
              << std::format(
                     " {:3}%\n",
                     static_cast<int>((static_cast<float>(i + 1) / static_cast<float>(uniqueTags.size())) * 100.f));
    std::cout << "\033[0;KProcessing " << repPath << "...\n";
    std::flush(std::cout);

    auto string = pool->resourceById(tag);
    if (string) {
      const auto outPath = (outputFolder / fileOut);
      if (!std::filesystem::exists(outPath)) {
        std::filesystem::create_directories(outPath.parent_path());
      }
      string->writeMetadata(outPath.generic_string(), repPath);
      if (string->typeCode() == axdl::primedep::AnimPOIData::ResourceType()) {
        (void)string->writeUncooked(outPath.generic_string());
      }
      manifest["Assets"].push_back(string->rawPath(repPath));
    }
    ++i;
  }
  std::flush(std::cout);
  printf("Complete!\n");
  athena::io::FileWriter writer((outputFolder / "Game.manifest").generic_string());
  writer.writeString(manifest.dump(4) + "\n");
#endif
  return 0;
}