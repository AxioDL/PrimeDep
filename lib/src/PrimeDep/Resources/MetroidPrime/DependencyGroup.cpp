#include "PrimeDep/Resources/MetroidPrime/DependencyGroup.hpp"

#include "PrimeDep/ResourcePool.hpp"
#include "PrimeDep/ResourceUtils.hpp"
#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
DependencyGroup::DependencyGroup(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  uint32_t depCount = in.readUint32Big();

  while (depCount--) {
    m_dependencies.emplace_back(ObjectTag32Big::Load<false>(in));
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
} // namespace axdl::primedep::MetroidPrime