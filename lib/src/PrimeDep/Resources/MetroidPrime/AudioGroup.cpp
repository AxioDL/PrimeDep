#include "PrimeDep/Resources/MetroidPrime/AudioGroup.hpp"

#include "PrimeDep/ResourcePool.hpp"

#include <athena/FileReader.hpp>
#include <athena/FileWriter.hpp>
#include <athena/MemoryReader.hpp>

#include <source_location>

namespace axdl::primedep::MetroidPrime {
AudioGroup::AudioGroup(const char* data, const std::size_t size) {
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

AudioGroup::AudioGroup(const nlohmann::ordered_json& in) {
  const auto p = std::filesystem::path(repPath());
  m_moduleDir = in.value("ModuleDir", "Audio/");
  m_moduleName = in.value("ModuleName", p.filename().generic_string());

  const auto poolFile = ResourcePool32Big::instance()->filePathFromRepPath(in.value("PoolFile", ""));
  const auto projFile = ResourcePool32Big::instance()->filePathFromRepPath(in.value("ProjectFile", ""));
  const auto samplesFile = ResourcePool32Big::instance()->filePathFromRepPath(in.value("SamplesFile", ""));
  const auto sampleDirFile = ResourcePool32Big::instance()->filePathFromRepPath(in.value("SampleDirectoryFile", ""));
  if (!exists(poolFile) || !exists(projFile) || !exists(samplesFile) || !exists(sampleDirFile)) {
    return;
  }

  {
    athena::io::FileReader reader(poolFile.generic_string());
    m_poolSize = reader.length();
    m_pool = std::make_unique<uint8_t[]>(m_poolSize);
    reader.readUBytesToBuf(m_pool.get(), m_poolSize);
  }

  {
    athena::io::FileReader reader(projFile.generic_string());
    m_projectSize = reader.length();
    m_project = std::make_unique<uint8_t[]>(m_poolSize);
    reader.readUBytesToBuf(m_project.get(), m_projectSize);
  }

  {
    athena::io::FileReader reader(samplesFile.generic_string());
    m_samplesSize = reader.length();
    m_samples = std::make_unique<uint8_t[]>(m_samplesSize);
    reader.readUBytesToBuf(m_samples.get(), m_samplesSize);
  }

  {
    athena::io::FileReader reader(sampleDirFile.generic_string());
    m_sampleDirSize = reader.length();
    m_sampleDir = std::make_unique<uint8_t[]>(m_sampleDirSize);
    reader.readUBytesToBuf(m_sampleDir.get(), m_sampleDirSize);
  }
}

bool AudioGroup::writeUncooked(const std::string_view path) const {
  const std::filesystem::path infoPath(GetRawPath(path));
  const std::filesystem::path poolPath(std::filesystem::path(path).replace_extension(".pool"));
  const std::filesystem::path projPath(std::filesystem::path(path).replace_extension(".proj"));
  const std::filesystem::path samplesPath(std::filesystem::path(path).replace_extension(".samp"));
  const std::filesystem::path sdirPath(std::filesystem::path(path).replace_extension(".sdir"));

  {
    nlohmann::ordered_json info;
    info["ModuleDir"] = m_moduleDir;
    info["ModuleName"] = m_moduleName;
    info["PoolFile"] = std::filesystem::path(repPath()).replace_extension(".pool").generic_string();
    info["ProjectFile"] = std::filesystem::path(repPath()).replace_extension(".proj").generic_string();
    info["SamplesFile"] = std::filesystem::path(repPath()).replace_extension(".samp").generic_string();
    info["SampleDirectoryFile"] = std::filesystem::path(repPath()).replace_extension(".sdir").generic_string();
    athena::io::FileWriter writer(infoPath.generic_string());
    std::string js = info.dump(4) + "\n";
    writer.writeString(js, js.length());
    if (writer.hasError()) {
      return false;
    }
  }

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

  return !writer.hasError();
}

nlohmann::ordered_json AudioGroup::metadata(const std::string_view path) const {
  nlohmann::ordered_json json = ITypedResource::metadata(path);
  json["ModuleDir"] = m_moduleDir;
  json["ModuleName"] = m_moduleName;
  return json;
}

std::shared_ptr<IResource> AudioGroup::loadCooked(const char* data, std::size_t size) {
  return std::make_shared<AudioGroup>(data, size);
}

std::shared_ptr<IResource> AudioGroup::ingest(const nlohmann::ordered_json& [[maybe_unused]] metadata,
                                              const std::string_view path) {
  athena::io::FileReader in(path);
  auto js = nlohmann::ordered_json::parse(in.readString());
  return std::make_shared<AudioGroup>(js);
}

} // namespace axdl::primedep::MetroidPrime
