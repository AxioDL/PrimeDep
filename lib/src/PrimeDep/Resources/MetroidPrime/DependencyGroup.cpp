#include "PrimeDep/Resources/MetroidPrime/DependencyGroup.hpp"

#include "PrimeDep/ResourcePool.hpp"

#include <athena/FileReader.hpp>
#include <athena/MemoryReader.hpp>

namespace axdl::primedep::MetroidPrime {
DependencyGroup::DependencyGroup(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  uint32_t depCount = in.readUint32Big();

  while (depCount--) {
    m_dependencies.emplace_back(ObjectTag32Big::Load<false>(in));
  }
}

DependencyGroup::DependencyGroup(const nlohmann::ordered_json& j) {
  if (!j.contains("Dependencies")) {
    return;
  }

  for (const auto& deps = j["Dependencies"]; const auto& dep : deps) {
    m_dependencies.emplace_back(ObjectTag32Big::Load(dep));
  }
}

bool DependencyGroup::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path).generic_string();
  athena::io::FileWriter out(p);
  out.writeUint32Big(m_dependencies.size());
  for (const auto& dependency : m_dependencies) {
    dependency.PutTo(out);
  }

  return !out.hasError();
}

bool DependencyGroup::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);

  nlohmann::ordered_json j;

  auto& dependencies = j["Dependencies"];

  for (const auto& dependency : m_dependencies) {
    dependency.PutTo(dependencies.emplace_back());
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = j.dump(4) + "\n";
  writer.writeString(js, js.length());

  return !writer.hasError();
}

std::shared_ptr<IResource> DependencyGroup::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<DependencyGroup>(ptr, size);
}

std::shared_ptr<IResource> DependencyGroup::ingest(const nlohmann::ordered_json& [[maybe_unused]] metadata,
                                                   const std::string_view path) {
  athena::io::FileReader in(path);
  auto js = nlohmann::ordered_json::parse(in.readString());
  if (!js.contains("Dependencies")) {
    return nullptr;
  }
  return std::make_shared<DependencyGroup>(js);
}
} // namespace axdl::primedep::MetroidPrime