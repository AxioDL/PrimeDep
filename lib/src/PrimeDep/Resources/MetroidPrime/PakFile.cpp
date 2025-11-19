#include "PrimeDep/Resources/MetroidPrime/PakFile.hpp"
#include "PrimeDep/ResourceFactory.hpp"
#include "PrimeDep/ResourceNameDatabase.hpp"
#include "PrimeDep/ResourcePool.hpp"

#include <athena/Compression.hpp>
#include <athena/FileReader.hpp>
#include <athena/FileWriter.hpp>

#include <fstream>

namespace axdl::primedep ::MetroidPrime {

bool PakFile::loadHeader() {
  athena::io::FileReader in(m_path);
  m_magic = in.readUint32Big();
  if (in.hasError()) {
    return false;
  }
  in.readUint32Big(); // Always 0
  if (in.hasError()) {
    return false;
  }
  uint32_t namedResourceCount = in.readUint32Big();
  if (in.hasError()) {
    return false;
  }
  while (namedResourceCount--) {
    m_namedResources.emplace_back(in);
    if (in.hasError()) {
      return false;
    }
  }
  uint32_t resourceCount = in.readUint32Big();
  if (in.hasError()) {
    return false;
  }
  while (resourceCount--) {
    m_resourceDescriptors.emplace_back(in);
    if (in.hasError()) {
      return false;
    }
  }

  return true;
}

std::shared_ptr<PakFile> PakFile::load(const std::string_view path) {
  auto ret = std::make_shared<PakFile>(path);
  if (!ret->loadHeader()) {
    return nullptr;
  }

  return ret;
}

bool PakFile::hasResource(const ObjectTag32Big& tag) {
  return std::ranges::find_if(m_resourceDescriptors, [&tag](const auto& descriptor) {
           return descriptor.assetId() == tag.id() && descriptor.type() == tag.type;
         }) != m_resourceDescriptors.end();
}

bool PakFile::hasNamedResource(std::string_view name) {
  return std::ranges::find_if(m_namedResources, [&name](const auto& descriptor) {
           return descriptor.name() == name;
         }) != m_namedResources.end();
}

ResourceDescriptor32Big PakFile::descriptorByName(std::string_view name) {
  const auto& nameDesc =
      std::ranges::find_if(m_namedResources, [&name](const auto& descriptor) { return descriptor.name() == name; });
  if (nameDesc == m_namedResources.end()) {
    return {};
  }

  const auto& desc = std::ranges::find_if(m_resourceDescriptors, [&nameDesc](const auto& descriptor) {
    return descriptor.assetId() == nameDesc->assetId() && descriptor.type() == nameDesc->type();
  });
  if (desc == m_resourceDescriptors.end()) {
    return {};
  }
  return *desc;
}

ResourceDescriptor32Big PakFile::descriptorById(const ObjectTag32Big& tag) {
  const auto& desc = std::ranges::find_if(m_resourceDescriptors, [&tag](const auto& descriptor) {
    return descriptor.assetId() == tag.id() && descriptor.type() == tag.type;
  });
  if (desc == m_resourceDescriptors.end()) {
    return {};
  }
  return *desc;
}

std::tuple<const char*, uint32_t> PakFile::loadData(const ResourceDescriptor32Big& desc) {
  if (!desc) {
    return {nullptr, 0};
  }

  if (!m_reader) {
    m_reader.emplace(m_path);
    if (!m_reader->isOpen()) {
      return {nullptr, 0};
    }
  }

  uint32_t size = desc.dataSize();

  m_reader->seek(desc.dataOffset(), athena::SeekOrigin::Begin);
  auto* data = new char[size];
  m_reader->readUBytesToBuf(data, size);

  if (desc.isCompressed()) {
    uint32_t decompressedSize = *reinterpret_cast<uint32_t*>(data);
    athena::utility::BigUint32(decompressedSize);
    const auto decompressedData = new char[decompressedSize];
    if (athena::io::Compression::decompressZlib(reinterpret_cast<const uint8_t*>(data + 4), desc.dataSize() - 4,
                                                reinterpret_cast<uint8_t*>(decompressedData),
                                                decompressedSize) != decompressedSize) {
      delete[] data;
      delete[] decompressedData;
      printf("Failed to decompress data for %.4s id 0x%08X\n", desc.type().toString().c_str(), desc.assetId().id);
      return {nullptr, 0};
    }
    delete[] data;
    data = decompressedData;
    size = decompressedSize;
  }

  return {data, size};
}

nlohmann::ordered_json PakFile::metadata() const {
  nlohmann::ordered_json ret;
  ret["PackageName"] = std::filesystem::path(path()).filename().replace_extension().generic_string();
  auto& namedResources = ret["NamedResources"];
  for (const auto& namedRes : m_namedResources) {
    nlohmann::ordered_json& descriptor = namedResources.emplace_back();
    descriptor["Name"] = namedRes.name();
    const auto tag = ObjectTag32Big{namedRes.type(), namedRes.assetId()};
    tag.PutTo(descriptor["Ref"]);
  }

  return ret;
}

} // namespace axdl::primedep::MetroidPrime