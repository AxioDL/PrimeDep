#include "PrimeDep/Resources/AudioGroup.hpp"

#include "athena/FileWriter.hpp"
#include "athena/MemoryReader.hpp"

#include <source_location>

namespace axdl::primedep {
AudioGroup::AudioGroup(const char* data, std::size_t size, const ResourceDescriptor32Big& desc) : ITypedResource(desc) {
  athena::io::MemoryReader in(data, size, true);
  m_moduleDir = in.readString();
  m_moduleName = in.readString();
  m_poolSize = in.readUint32Big();
  m_pool = std::make_unique<uint8_t[]>(m_poolSize);
  in.readUBytesToBuf(m_pool.get(), m_poolSize);
  m_projectSize = in.readUint32Big();
  m_project = std::make_unique<uint8_t[]>(m_projectSize);
  in.readUBytesToBuf(m_project.get(), m_projectSize);
  m_samplesSize = in.readUint32Big();
  m_samples = std::make_unique<uint8_t[]>(m_samplesSize);
  in.readUBytesToBuf(m_samples.get(), m_samplesSize);
  m_sampleDirSize = in.readUint32Big();
  m_sampleDir = std::make_unique<uint8_t[]>(m_sampleDirSize);
  in.readUBytesToBuf(m_sampleDir.get(), m_sampleDirSize);
}

std::shared_ptr<IResource> AudioGroup::loadCooked(const char* data, std::size_t size,
                                                  const ResourceDescriptor32Big& desc) {
  return std::make_shared<AudioGroup>(data, size, desc);
}

bool AudioGroup::writeUncooked(const std::string_view path) const {
  const std::filesystem::path poolPath(std::filesystem::path(path).replace_extension(".pool"));
  const std::filesystem::path projPath(std::filesystem::path(path).replace_extension(".proj"));
  const std::filesystem::path samplesPath(std::filesystem::path(path).replace_extension(".samp"));
  const std::filesystem::path sdirPath(std::filesystem::path(path).replace_extension(".sdir"));

  {
    auto writer = athena::io::FileWriter(poolPath.generic_string());
    writer.writeUBytes(m_pool.get(), m_poolSize);
    if (writer.hasError()) {
      return false;
    }
  }
  {
    auto writer = athena::io::FileWriter(projPath.generic_string());
    writer.writeUBytes(m_project.get(), m_projectSize);
    if (writer.hasError()) {
      return false;
    }
  }
  {
    auto writer = athena::io::FileWriter(samplesPath.generic_string());
    writer.writeUBytes(m_samples.get(), m_samplesSize);
    if (writer.hasError()) {
      return false;
    }
  }
  {
    auto writer = athena::io::FileWriter(sdirPath.generic_string());
    writer.writeUBytes(m_sampleDir.get(), m_sampleDirSize);
    if (writer.hasError()) {
      return false;
    }
  }

  return true;
}

bool AudioGroup::writeCooked(const std::string_view path) const {
  std::filesystem::path cookedPath(path);
  while (cookedPath.has_extension()) {
    cookedPath = cookedPath.replace_extension();
  }
  cookedPath.replace_extension(".agsc");

  athena::io::FileWriter writer(cookedPath.generic_string());
  writer.writeString(m_moduleDir);
  writer.writeString(m_moduleName);
  writer.writeUint32Big(m_poolSize);
  writer.writeUBytes(m_pool.get(), m_poolSize);
  writer.writeUint32Big(m_projectSize);
  writer.writeUBytes(m_project.get(), m_projectSize);
  writer.writeUint32Big(m_samplesSize);
  writer.writeUBytes(m_samples.get(), m_samplesSize);
  writer.writeUint32Big(m_sampleDirSize);
  writer.writeUBytes(m_sampleDir.get(), m_sampleDirSize);

  writer.fill(static_cast<uint8_t>(0xFF), ROUND_UP_32(writer.length()) - writer.length());
  return true;
}

nlohmann::ordered_json AudioGroup::metadata(const std::string_view path) const {
  nlohmann::ordered_json json = ITypedResource::metadata(path);
  ;
  json["ModuleDir"] = m_moduleDir;
  json["ModuleName"] = m_moduleName;
  json["PoolFile"] = "$/" + std::filesystem::path(path).replace_extension(".pool").generic_string();
  json["ProjectFile"] = "$/" + std::filesystem::path(path).replace_extension(".proj").generic_string();
  json["SamplesFile"] = "$/" + std::filesystem::path(path).replace_extension(".samp").generic_string();
  json["SampleDirectoryFile"] = "$/" + std::filesystem::path(path).replace_extension(".sdir").generic_string();
  return json;
}

} // namespace axdl::primedep
