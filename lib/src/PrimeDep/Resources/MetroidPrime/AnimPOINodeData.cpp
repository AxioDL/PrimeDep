#include "PrimeDep/Resources/MetroidPrime/AnimPOIData.hpp"

#include "athena/FileReader.hpp"
#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AnimPOIData::AnimPOIData(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);

  m_version = in.readUint32Big();

  auto boolCount = in.readUint32Big();
  while (boolCount--) {
    m_boolNodes.emplace_back(in);
  }
  auto int32NodeCount = in.readUint32Big();
  while (int32NodeCount--) {
    m_int32Nodes.emplace_back(in);
  }

  auto particleNodeCount = in.readUint32Big();
  while (particleNodeCount--) {
    m_particleNodes.emplace_back(in);
  }
  if (m_version >= 2) {
    auto soundNodeCount = in.readUint32Big();
    while (soundNodeCount--) {
      m_soundNodes.emplace_back(in);
    }
  }
}

AnimPOIData::AnimPOIData(const nlohmann::ordered_json& in) : m_version(in.value("Version", m_version)) {
  if (in.contains("BoolNodes")) {
    for (const auto& boolNodes = in.at("BoolNodes"); const auto& node : boolNodes) {
      m_boolNodes.emplace_back(node);
    }
  }

  if (in.contains("Int32Nodes")) {
    for (const auto& int32Nodes = in.at("Int32Nodes"); const auto& node : int32Nodes) {
      m_int32Nodes.emplace_back(node);
    }
  }

  if (in.contains("ParticleNodes")) {
    for (const auto& particleNodes = in.at("ParticleNodes"); const auto& node : particleNodes) {
      m_particleNodes.emplace_back(node);
    }
  }

  if (m_version >= 2 && in.contains("SoundNodes")) {
    for (const auto& soundNodes = in.at("SoundNodes"); const auto& node : soundNodes) {
      m_soundNodes.emplace_back(node);
    }
  }
}

bool AnimPOIData::writeCooked(std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());

  out.writeUint32Big(m_version);
  out.writeUint32Big(m_boolNodes.size());
  for (const auto& boolNode : m_boolNodes) {
    boolNode.PutTo(out);
  }

  out.writeUint32Big(m_int32Nodes.size());
  for (const auto& int32Node : m_int32Nodes) {
    int32Node.PutTo(out);
  }

  out.writeUint32Big(m_particleNodes.size());
  for (const auto& particleNode : m_particleNodes) {
    particleNode.PutTo(out);
  }
  if (m_version >= 2) {
    out.writeUint32Big(m_soundNodes.size());
    for (const auto& soundNode : m_soundNodes) {
      soundNode.PutTo(out);
    }
  }
  return !out.hasError();
}

bool AnimPOIData::writeUncooked(const std::string_view path) const {
  const std::filesystem::path p = GetRawPath(path);

  nlohmann::ordered_json j;
  j["Version"] = m_version;

  auto& boolNodes = j["BoolNodes"];
  for (const auto& node : m_boolNodes) {
    node.PutTo(boolNodes.emplace_back());
  }

  auto& int32Nodes = j["Int32Nodes"];
  for (const auto& node : m_int32Nodes) {
    node.PutTo(int32Nodes.emplace_back());
  }

  auto& particleNodes = j["ParticleNodes"];
  for (const auto& node : m_particleNodes) {
    node.PutTo(particleNodes.emplace_back());
  }

  if (m_version >= 2) {
    auto& soundNodes = j["SoundNodes"];
    for (const auto& node : m_soundNodes) {
      auto& n = soundNodes.emplace_back();
      node.PutTo(n);
    }
  }
  athena::io::FileWriter writer(p.generic_string());
  std::string js = j.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> AnimPOIData::ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
  const auto p = GetRawPath(path);
  athena::io::FileReader in(p.generic_string());
  auto js = nlohmann::ordered_json::parse(in.readString());

  return std::make_shared<AnimPOIData>(js);
}

std::shared_ptr<IResource> AnimPOIData::loadCooked(const char* ptr, const std::size_t size) {

  return std::make_shared<AnimPOIData>(ptr, size);
}

} // namespace axdl::primedep::MetroidPrime