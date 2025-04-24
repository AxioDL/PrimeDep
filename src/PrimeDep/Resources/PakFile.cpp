#include "PrimeDep/Resources/PakFile.hpp"

#include "athena/Compression.hpp"
#include "../../../include/PrimeDep/ResourceFactory.hpp"
#include "athena/FileWriter.hpp"

#include <athena/FileReader.hpp>

namespace axdl::primedep {

bool PakFile32Big::loadHeader() {
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
std::optional<PakFile32Big> PakFile32Big::load(const std::string_view path, const ResourceFactory& factory) {
  PakFile32Big ret(path, factory);
  if (!ret.loadHeader()) {
    return std::nullopt;
  }

  return ret;
}

ResourceDescriptor32Big PakFile32Big::resourceDescriptorByName(std::string_view name) const {
  const auto& namedDesc =
      std::ranges::find_if(m_namedResources, [&name](const NamedResource32Big& desc) { return desc.name() == name; });

  if (namedDesc == m_namedResources.end()) {
    return {};
  }

  const auto& resourceDesc =
      std::ranges::find_if(m_resourceDescriptors, [&namedDesc](const ResourceDescriptor32Big& desc) {
        return desc.assetId() == namedDesc->assetId();
      });
  if (resourceDesc == m_resourceDescriptors.end()) {
    return {};
  }
  return *resourceDesc;
}

std::shared_ptr<IResource> PakFile32Big::resourceById(const ObjectTag32Big& tag) {
  const auto& resourceDesc = std::ranges::find_if(m_resourceDescriptors, [&tag](const ResourceDescriptor32Big& desc) {
    return desc.type() == tag.type && desc.assetId() == tag.id;
  });
  if (resourceDesc == m_resourceDescriptors.end()) {
    return nullptr;
  }
  return resourceByDescriptor(*resourceDesc);
}

std::shared_ptr<IResource> PakFile32Big::resourceByDescriptor(const ResourceDescriptor32Big& desc) {
  if (!desc) {
    return nullptr;
  }

  if (m_loadedResources.contains(desc) && m_loadedResources.at(desc) != nullptr) {
    const auto type = desc.type();
    printf("Found already loaded data for %.4s id 0x%08X\n", reinterpret_cast<const char*>(&type), desc.assetId().id);
    return m_loadedResources.at(desc);
  }
  const auto& factory = m_factory.factory(desc.type());
  if (!factory) {
    return nullptr;
  }

  printf("Found factory for %.4s id 0x%08X\n", desc.type().toString().c_str(), desc.assetId().id);
  if (!m_reader) {
    m_reader.emplace(m_path);
    if (!m_reader->isOpen()) {
      return {};
    }
  }

  m_reader->seek(desc.dataOffset(), athena::SeekOrigin::Begin);
  auto* data = new char[desc.dataSize()];
  m_reader->readUBytesToBuf(data, desc.dataSize());
  if (m_reader->hasError()) {
    printf("Failed to read data for %.4s id 0x%08X\n", desc.type().toString().c_str(), desc.assetId().id);
    delete[] data;
    return {};
  }

  uint32_t size = desc.dataSize();

  if (desc.isCompressed()) {
    uint32_t decompressedSize = *reinterpret_cast<uint32_t*>(data);
    athena::utility::BigUint32(decompressedSize);
    const auto decompressedData = new char[decompressedSize];
    if (athena::io::Compression::decompressZlib(reinterpret_cast<const atUint8*>(data + 4), desc.dataSize() - 4,
                                                reinterpret_cast<atUint8*>(decompressedData),
                                                decompressedSize) != decompressedSize) {
      delete[] data;
      delete[] decompressedData;
      printf("Failed to decompress data for %.4s id 0x%08X\n", desc.type().toString().c_str(), desc.assetId().id);
      return nullptr;
    }
    delete[] data;
    data = decompressedData;
    size = decompressedSize;
  }

  auto res = factory(data, size);
  if (res) {
    m_loadedResources[desc] = res;
  }

  return res;
}

void PakFile32Big::writeUncompressedToFileSystem(std::string_view path, const ResourceDescriptor32Big& desc) {
  if (!desc) {
    return;
  }

  if (!m_reader) {
    m_reader.emplace(m_path);
    if (!m_reader->isOpen()) {
      return;
    }
  }

  m_reader->seek(desc.dataOffset(), athena::SeekOrigin::Begin);
  auto* data = new char[desc.dataSize()];
  m_reader->readUBytesToBuf(data, desc.dataSize());
  if (m_reader->hasError()) {
    printf("Failed to read data for %.4s id 0x%08X\n", desc.type().toString().c_str(), desc.assetId().id);
    delete[] data;
    return;
  }

  uint32_t size = desc.dataSize();

  if (desc.isCompressed()) {
    uint32_t decompressedSize = *reinterpret_cast<uint32_t*>(data);
    athena::utility::BigUint32(decompressedSize);
    const auto decompressedData = new char[decompressedSize];
    if (athena::io::Compression::decompressZlib(reinterpret_cast<const atUint8*>(data + 4), desc.dataSize() - 4,
                                                reinterpret_cast<atUint8*>(decompressedData),
                                                decompressedSize) != decompressedSize) {
      delete[] data;
      delete[] decompressedData;
      printf("Failed to decompress data for %.4s id 0x%08X\n", desc.type().toString().c_str(), desc.assetId().id);
      return;
    }
    delete[] data;
    data = decompressedData;
    size = decompressedSize;
  }

  athena::io::FileWriter writer(path);
  writer.writeBytes(data, size);
}

} // namespace axdl::primedep