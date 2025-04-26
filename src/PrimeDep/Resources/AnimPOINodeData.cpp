#include "PrimeDep/Resources/AnimPOIData.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep {
AnimPOIData::AnimPOIData(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
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

bool AnimPOIData::writeUncooked(std::string_view path) const {
  std::filesystem::path p = GetRawPath(path);

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

std::shared_ptr<IResource> AnimPOIData::loadCooked(const char* ptr, const std::size_t size,
                                                   const ResourceDescriptor32Big& desc) {

  return std::make_shared<AnimPOIData>(ptr, size, desc);
}

} // namespace axdl::primedep