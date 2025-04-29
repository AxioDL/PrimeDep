#pragma once

#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/IResource.hpp"
#include "PrimeDep/IResourceSource.hpp"
#include "PrimeDep/NamedResource.hpp"
#include "PrimeDep/ObjectTag.hpp"
#include "PrimeDep/ResourceDescriptor.hpp"

#include <athena/FileReader.hpp>

#include <map>

namespace axdl::primedep::MetroidPrime {
class PakFile final : public IResourceSource<ResourceDescriptor32Big, ObjectTag32Big> {
public:
  explicit PakFile(const std::string_view path) : m_path(path) {}

  static constexpr uint32_t kMagicNumber = 0x00030005;
  static std::shared_ptr<PakFile> load(std::string_view path);

  PakFile(const PakFile& other)
  : m_magic(other.m_magic)
  , m_path(other.m_path)
  , m_namedResources(other.m_namedResources)
  , m_resourceDescriptors(other.m_resourceDescriptors) {};

  PakFile(PakFile&& other) noexcept
  : m_magic(other.m_magic)
  , m_path(std::move(other.m_path))
  , m_namedResources(std::move(other.m_namedResources))
  , m_resourceDescriptors(std::move(other.m_resourceDescriptors)) {}

  explicit operator bool() const { return m_magic == kMagicNumber; }

  [[nodiscard]] const std::string& path() const override { return m_path; }

  [[nodiscard]] std::size_t namedResourceCount() const { return m_namedResources.size(); }
  [[nodiscard]] const std::vector<NamedResource32Big>& namedResources() const { return m_namedResources; }
  [[nodiscard]] std::size_t resourceDescriptorCount() const { return m_resourceDescriptors.size(); }
  [[nodiscard]] const std::vector<ResourceDescriptor32Big>& resourceDescriptors() const {
    return m_resourceDescriptors;
  }

  bool hasResource(const ObjectTag32Big& tag) override;
  bool hasNamedResource(std::string_view name) override;

  ResourceDescriptor32Big descriptorByName(std::string_view name) override;
  ResourceDescriptor32Big descriptorById(const ObjectTag32Big& tag) override;

  std::tuple<const char*, uint32_t> loadData(const ResourceDescriptor32Big& desc) override;

  std::vector<ObjectTag32Big> tagsByType(const FourCC& type) override {
    std::vector<ObjectTag32Big> tags;
    for (const auto& res : m_resourceDescriptors) {
      if (res.type() == type || type == kInvalidFourCC) {
        tags.emplace_back(res.type(), res.assetId());
      }
    }
    return tags;
  }

  [[nodiscard]] nlohmann::ordered_json metadata() const override;

  void writeMetadata(const std::string_view path) const {
    auto p = std::filesystem::path(path);
    while (p.has_extension()) {
      p.replace_extension();
    }

    p.replace_extension(".prj");

    athena::io::FileWriter writer(p.generic_string());
    const auto str = metadata().dump(4) + "\n";
    writer.writeString(str, str.length());
  }

private:
  bool loadHeader();
  uint32_t m_magic{};
  std::string m_path;
  std::optional<athena::io::FileReader> m_reader = std::nullopt;
  std::vector<NamedResource32Big> m_namedResources;
  std::vector<ResourceDescriptor32Big> m_resourceDescriptors;
};
} // namespace axdl::primedep::MetroidPrime
