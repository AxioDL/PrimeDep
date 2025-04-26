#include <PrimeDep/ResourceFactory.hpp>
#include <PrimeDep/ResourceNameDatabase.hpp>
#include <PrimeDep/ResourcePool.hpp>
#include <PrimeDep/Resources/MetroidPrime/AiFiniteStateMachine.hpp>
#include <PrimeDep/Resources/MetroidPrime/AnimPOIData.hpp>
#include <PrimeDep/Resources/MetroidPrime/AnimSource.hpp>
#include <PrimeDep/Resources/MetroidPrime/AudioGroup.hpp>
#include <PrimeDep/Resources/MetroidPrime/AudioTranslationTable.hpp>
#include <PrimeDep/Resources/MetroidPrime/CharLayoutInfo.hpp>
#include <PrimeDep/Resources/MetroidPrime/CollidableOBBTreeGroup.hpp>
#include <PrimeDep/Resources/MetroidPrime/CollisionResponseData.hpp>
#include <PrimeDep/Resources/MetroidPrime/DecalData.hpp>
#include <PrimeDep/Resources/MetroidPrime/GuiFrame.hpp>
#include <PrimeDep/Resources/MetroidPrime/MapArea.hpp>
#include <PrimeDep/Resources/MetroidPrime/MapWorld.hpp>
#include <PrimeDep/Resources/MetroidPrime/MetroidArea.hpp>
#include <PrimeDep/Resources/MetroidPrime/MetroidWorld.hpp>
#include <PrimeDep/Resources/MetroidPrime/Model.hpp>
#include <PrimeDep/Resources/MetroidPrime/PakFile.hpp>
#include <PrimeDep/Resources/MetroidPrime/Particle.hpp>
#include <PrimeDep/Resources/MetroidPrime/ParticleElectric.hpp>
#include <PrimeDep/Resources/MetroidPrime/ParticleSwoosh.hpp>
#include <PrimeDep/Resources/MetroidPrime/PathFindArea.hpp>
#include <PrimeDep/Resources/MetroidPrime/ProjectileWeapon.hpp>
#include <PrimeDep/Resources/MetroidPrime/RasterFont.hpp>
#include <PrimeDep/Resources/MetroidPrime/ScannableObjectInfo.hpp>
#include <PrimeDep/Resources/MetroidPrime/SkinRules.hpp>
#include <PrimeDep/Resources/MetroidPrime/StringTable.hpp>
#include <PrimeDep/Resources/MetroidPrime/Texture.hpp>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
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
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::StringTable>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::Model>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::Texture>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::SkinRules>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::AnimSource>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::CharLayoutInfo>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::CollisionResponseData>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::ParticleSwoosh>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::Particle>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::ParticleElectric>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::ProjectileWeapon>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::GuiFrame>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::RasterFont>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::ScannableObjectInfo>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::AnimPOIData>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::AiFiniteStateMachine>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::AudioGroup>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::CollidableOBBTreeGroup>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::DecalData>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::AudioTranslationTable>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::PathFindArea>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MapWorld>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MetroidArea>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MapArea>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MapUniverse>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MidiData>(factory);
  // axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::SaveWorld>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MetroidWorld>(factory);
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
  std::cout << "Loading ResourceDB...";
  std::flush(std::cout);
  axdl::primedep::ResourceNameDatabase::instance().load(
      (executableDirectory() / ".." / "ResourceDB.json").generic_string());
  std::cout << "Done!" << std::endl;
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

  std::shared_ptr<axdl::primedep::MetroidPrime::PakFile> metroid1;
  for (const auto& entry : std::filesystem::directory_iterator(inputFolder)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    std::string extension = entry.path().extension();
    athena::utility::tolower(extension);

    if (extension != ".pak") {
      continue;
    }
    auto pak = axdl::primedep::MetroidPrime::PakFile::load(entry.path().generic_string());
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
      (void)string->writeUncooked(outPath.generic_string());
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