#include "PackagePrime.hpp"

#include "PrimeDep/ResourceFactory.hpp"
#include "PrimeDep/ResourcePool.hpp"

#include <PrimeDep/Resources/MetroidPrime/AiFiniteStateMachine.hpp>
#include <PrimeDep/Resources/MetroidPrime/AnimCharacterSet.hpp>
#include <PrimeDep/Resources/MetroidPrime/AnimPOIData.hpp>
#include <PrimeDep/Resources/MetroidPrime/AnimSource.hpp>
#include <PrimeDep/Resources/MetroidPrime/AudioGroup.hpp>
#include <PrimeDep/Resources/MetroidPrime/AudioTranslationTable.hpp>
#include <PrimeDep/Resources/MetroidPrime/CharLayoutInfo.hpp>
#include <PrimeDep/Resources/MetroidPrime/CollidableOBBTreeGroup.hpp>
#include <PrimeDep/Resources/MetroidPrime/CollisionResponseData.hpp>
#include <PrimeDep/Resources/MetroidPrime/DecalData.hpp>
#include <PrimeDep/Resources/MetroidPrime/DependencyGroup.hpp>
#include <PrimeDep/Resources/MetroidPrime/GuiFrame.hpp>
#include <PrimeDep/Resources/MetroidPrime/MapArea.hpp>
#include <PrimeDep/Resources/MetroidPrime/MapUniverse.hpp>
#include <PrimeDep/Resources/MetroidPrime/MapWorld.hpp>
#include <PrimeDep/Resources/MetroidPrime/MetroidArea.hpp>
#include <PrimeDep/Resources/MetroidPrime/MetroidWorld.hpp>
#include <PrimeDep/Resources/MetroidPrime/MidiData.hpp>
#include <PrimeDep/Resources/MetroidPrime/Model.hpp>
#include <PrimeDep/Resources/MetroidPrime/PakFile.hpp>
#include <PrimeDep/Resources/MetroidPrime/Particle.hpp>
#include <PrimeDep/Resources/MetroidPrime/ParticleElectric.hpp>
#include <PrimeDep/Resources/MetroidPrime/ParticleSwoosh.hpp>
#include <PrimeDep/Resources/MetroidPrime/PathFindArea.hpp>
#include <PrimeDep/Resources/MetroidPrime/ProjectileWeapon.hpp>
#include <PrimeDep/Resources/MetroidPrime/RasterFont.hpp>
#include <PrimeDep/Resources/MetroidPrime/SaveWorld.hpp>
#include <PrimeDep/Resources/MetroidPrime/ScannableObjectInfo.hpp>
#include <PrimeDep/Resources/MetroidPrime/SkinRules.hpp>
#include <PrimeDep/Resources/MetroidPrime/StringTable.hpp>
#include <PrimeDep/Resources/MetroidPrime/Texture.hpp>

#include <iostream>
#include <netdb.h>
#include <nlohmann/json.hpp>

#include <athena/Compression.hpp>
#include <athena/FileReader.hpp>
#include <athena/FileWriter.hpp>
#include <athena/MemoryWriter.hpp>

void addFactories(axdl::primedep::ResourceFactory32Big& factory) {
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::StringTable>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::Model>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::Texture>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::SkinRules>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::AnimSource>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::CharLayoutInfo>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::AnimCharacterSet>(factory);
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
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MapUniverse>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MidiData>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::SaveWorld>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::MetroidWorld>(factory);
  axdl::primedep::RegisterFactory32Big<axdl::primedep::MetroidPrime::DependencyGroup>(factory);
}

struct CompressionInfo {
  axdl::primedep::FourCC type;
  bool compress{false};
  bool minium1k{false};
};

constexpr std::array kCompressionInfoMap = {
    CompressionInfo{FOURCC('TXTR'), true, false}, CompressionInfo{FOURCC('CMDL'), true, false},
    CompressionInfo{FOURCC('CSKR'), true, false}, CompressionInfo{FOURCC('ANCS'), true, false},
    CompressionInfo{FOURCC('ANIM'), true, false}, CompressionInfo{FOURCC('FONT'), true, false},
    CompressionInfo{FOURCC('PART'), true, true},  CompressionInfo{FOURCC('ELSC'), true, true},
    CompressionInfo{FOURCC('SWHC'), true, true},  CompressionInfo{FOURCC('WPSC'), true, true},
    CompressionInfo{FOURCC('DPSC'), true, true},  CompressionInfo{FOURCC('SRSC'), true, true},
};

void writeResource(athena::io::MemoryCopyWriter& resourceBuffer,
                   std::vector<axdl::primedep::ResourceDescriptor32Big>& descriptors, const axdl::primedep::FourCC type,
                   const std::shared_ptr<axdl::primedep::IResource>& res, const std::filesystem::path& cookedPath) {
  std::unique_ptr<uint8_t[]> buffer;
  athena::io::FileReader reader(cookedPath.generic_string());
  uint64_t size = reader.length();
  buffer = std::make_unique<uint8_t[]>(size);
  reader.readUBytesToBuf(buffer.get(), size);

  bool compressed{false};
  const auto compInfo =
      std::ranges::find_if(kCompressionInfoMap, [&type](const CompressionInfo& info) { return type == info.type; });
  if (compInfo != kCompressionInfoMap.end() && compInfo->compress) {
    auto compBuffer = std::make_unique<uint8_t[]>(size);
    const auto compLen = athena::io::Compression::compressZlib(buffer.get(), size, compBuffer.get(), size);
    if (compLen > 0 && (compLen >= 0x400 || !compInfo->minium1k)) {
      buffer = std::move(compBuffer);
      size = compLen;
      compressed = true;
    }
  }

  const auto curPos = resourceBuffer.position();
  resourceBuffer.fill(static_cast<uint8_t>(0xFF), ROUND_UP_32(size));
  resourceBuffer.seek(curPos, athena::SeekOrigin::Begin);
  resourceBuffer.writeBytes(buffer.get(), size);
  resourceBuffer.seekAlign32();
  auto& desc = descriptors.emplace_back();
  desc.setType(type);
  desc.setAssetId(res->assetId32Big());
  desc.setCompressed(compressed);
  desc.setDataSize(ROUND_UP_32(size));
  desc.setDataOffset(curPos);
}
bool packagePrime(const std::filesystem::path& packageDefPath, const std::filesystem::path& repPath,
                  const std::filesystem::path& outPath) {
  athena::io::FileReader metaReader(packageDefPath.generic_string());
  nlohmann::ordered_json packageMeta = nlohmann::ordered_json::parse(metaReader.readString());
  if (!packageMeta.contains("PackageName")) {
    std::cerr << "Package Definition missing PackageName" << std::endl;
    return false;
  }

  if (!packageMeta.contains("NamedResources")) {
    std::cerr << "Package Definition missing NamedResources, package files require at least 1 named resource"
              << std::endl;
    return false;
  }

  std::cout << "Creating package for " << packageMeta["PackageName"] << std::endl;
  std::filesystem::path packagePath(outPath / packageMeta.value("PackageName", ""));
  packagePath.replace_extension(".pak");

  axdl::primedep::ResourceFactory32Big factory;
  addFactories(factory);
  axdl::primedep::ResourcePool32Big pool(repPath);
  pool.setFactory(factory);

  axdl::primedep::MetroidPrime::PakFile pakFile(packagePath.generic_string());

  const auto& namedResources = packageMeta["NamedResources"];

  athena::io::MemoryCopyWriter resourceBuffer;
  std::vector<axdl::primedep::ResourceDescriptor32Big> descriptors;

  for (const auto& namedResource : namedResources) {
    const auto name = namedResource.value("Name", "");
    if (name.empty()) {
      std::cerr << "NamedResource entry malformed: missing `Name` value" << std::endl;
      return false;
    }
    std::string repPath;
    axdl::primedep::FourCC type;
    if (!namedResource.contains("Ref")) {
      type = axdl::primedep::FourCC(namedResource.value("Type", axdl::primedep::kInvalidFourCC.toString()));
      if (type == axdl::primedep::kInvalidFourCC) {
        std::cerr << "Invalid named resource definition `" << name << "` Type value missing" << std::endl;
        return false;
      }

      repPath = namedResource.value("File", "");
      if (repPath.empty()) {
        std::cerr << "Invalid named resource definition `" << name << "` missing rep path" << std::endl;
        return false;
      }
    } else {
      const auto tag = axdl::primedep::ObjectTag32Big::Load(namedResource["Ref"]);
      repPath = tag.repPath();
      type = tag.type;
    }

    const auto res = pool.ingestResourceByRepPath(repPath, type);
    if (!res) {
      std::cerr << "Unable to load named resource `" << name << "` not adding..." << std::endl;
      continue;
    }
    pakFile.addNamedResource(name, type, res->assetId32Big());

    const auto cookedPath = pool.filePathFromRepPath(pool.cookedRepPathFromRawRepPath(repPath, type));
    if (!exists(cookedPath)) {
      std::cout << "Cooking asset for " << name << std::endl;
      if (!res->writeCooked(cookedPath.generic_string())) {
        std::cerr << "Unable to cook named resource `" << name << "`" << std::endl;
      }
    }

    if (const auto children = res->children()) {
      for (const auto& child : *children) {
        writeResource(resourceBuffer, descriptors, child->typeCode(), child, cookedPath);
      }
    }

    writeResource(resourceBuffer, descriptors, type, res, cookedPath);
  }

  if (descriptors.empty()) {
    std::cerr << "Unable to find any resources for `" << std::string(packageMeta["PackageName"]) << "` bailing"
              << std::endl;
    return false;
  }

  athena::io::FileWriter pakWriter(packagePath.generic_string());
  pakWriter.writeUint32Big(axdl::primedep::MetroidPrime::PakFile::kMagicNumber);
  pakWriter.writeUint32Big(0);
  pakWriter.writeUint32Big(pakFile.namedResourceCount());

  for (const auto& namedResource : pakFile.namedResources()) {
    pakWriter.writeBytes(namedResource.type().fcc, 4);
    namedResource.assetId().PutTo(pakWriter);
    pakWriter.writeUint32Big(namedResource.name().length());
    pakWriter.writeString(namedResource.name(), namedResource.name().length());
  }

  const auto descPos = pakWriter.position();
  pakWriter.fill(static_cast<uint8_t>(0xFF), descriptors.size() * 20);
  pakWriter.seekAlign32();

  const auto dataPos = pakWriter.position();

  pakWriter.writeBytes(resourceBuffer.data(), resourceBuffer.length());

  pakWriter.seek(descPos, athena::SeekOrigin::Begin);
  pakWriter.writeUint32Big(descriptors.size());
  for (const auto& descriptor : descriptors) {
    pakWriter.writeUint32Big(descriptor.isCompressed());
    pakWriter.writeBytes(descriptor.type().fcc, 4);
    descriptor.assetId().PutTo(pakWriter);
    pakWriter.writeUint32Big(descriptor.dataSize());
    pakWriter.writeUint32Big(descriptor.dataOffset() + dataPos);
  }

  return true;
}
