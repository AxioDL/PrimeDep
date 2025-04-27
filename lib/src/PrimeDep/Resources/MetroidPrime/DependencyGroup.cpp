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

bool DependencyGroup::writeUncooked(std::string_view path) const {
  auto p = std::filesystem::path(path);
  while (p.has_extension()) {
    p.replace_extension();
  }
  p.replace_extension(RawExtension());

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