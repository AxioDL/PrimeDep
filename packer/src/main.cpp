#include "PackagePrime.hpp"

#include <PrimeDep/ResourceFactory.hpp>
#include <PrimeDep/Resources/MetroidPrime/PakFile.hpp>

#include <argparse/argparse.hpp>
#include <magic_enum/magic_enum.hpp>

enum class Game {
  Prime,
  Echoes,
  Corruption,
  Invalid,
};

#include <iostream>
#include <tiny_gltf.h>
#include <vector>

int main(const int argc, const char* argv[]) {
  argparse::ArgumentParser program("packer", "0.0.1");

  program.add_argument("package_source").help("Package definition file (e.g Metroid1.prj)");
  program.add_argument("rep_path").help("Source asset path");
  program.add_argument("output_folder").help("Destination folder");

  program.add_argument("--game")
      .help("Target game to create a package for")
      .default_value("prime")
      .choices("prime", "echoes", "corruption");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cout << program << std::endl;
    return EXIT_FAILURE;
  }

  const auto game = magic_enum::enum_cast<Game>(program.get<std::string>("--game"), magic_enum::case_insensitive)
                        .value_or(Game::Invalid);

  if (game == Game::Invalid) {
    return EXIT_FAILURE;
  }

  const auto packageDefPath = std::filesystem::path(program.get<std::string>("package_source"));
  const auto repPath = std::filesystem::path(program.get<std::string>("rep_path"));
  const auto outputFolder = std::filesystem::path(program.get<std::string>("output_folder"));
  if (!std::filesystem::exists(packageDefPath)) {
    std::cerr << "Package definition file does not exist" << std::endl;
    return EXIT_FAILURE;
  }

  if (!std::filesystem::exists(repPath)) {
    std::cerr << "Asset rep path does not exist" << std::endl;
    return EXIT_FAILURE;
  }

  if (!std::filesystem::exists(outputFolder) && !std::filesystem::create_directories(outputFolder)) {
    std::cerr << "Destination folder could not be created" << std::endl;
    return EXIT_FAILURE;
  }

  switch (game) {
  case Game::Prime:
    if (!packagePrime(packageDefPath, repPath, outputFolder)) {
      std::cerr << "Package could not be created" << std::endl;
      return EXIT_FAILURE;
    }
    break;
  case Game::Echoes:
    std::cerr << "Echoes not supported yet" << std::endl;
    // packageEchoes(packageDefPath, repPath, outputFolder);
    break;
  case Game::Corruption:
    std::cerr << "Corruption not supported yet" << std::endl;
    // packageCorruption(packageDefPath, repPath, outputFolder);
    break;
  default:
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}