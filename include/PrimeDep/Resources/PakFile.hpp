#pragma once

#include "PrimeDep/IResource.hpp"
#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/NamedResource.hpp"
#include "PrimeDep/ObjectTag.hpp"
#include "PrimeDep/ResourceDescriptor.hpp"

#include <athena/FileReader.hpp>

#include <map>

namespace axdl::primedep {
class ResourceFactory;

class PakFile32Big final {
public:
  static constexpr uint32_t kMagicNumber = 0x00030005;
  static std::optional<PakFile32Big> load(std::string_view path, const ResourceFactory& factory);

  PakFile32Big(const PakFile32Big& other)
  : m_factory(other.m_factory)
  , m_magic(other.m_magic)
  , m_path(other.m_path)
  , m_namedResources(other.m_namedResources)
  , m_resourceDescriptors(other.m_resourceDescriptors)
  , m_loadedResources(other.m_loadedResources) {};

  PakFile32Big(PakFile32Big&& other) noexcept
  : m_factory(other.m_factory)
  , m_magic(other.m_magic)
  , m_path(std::move(other.m_path))
  , m_namedResources(std::move(other.m_namedResources))
  , m_resourceDescriptors(std::move(other.m_resourceDescriptors))
  , m_loadedResources(std::move(other.m_loadedResources)) {}

  explicit operator bool() const { return m_magic == kMagicNumber; }

  [[nodiscard]] const std::string& path() const { return m_path; }

  [[nodiscard]] std::size_t namedResourceCount() const { return m_namedResources.size(); }
  [[nodiscard]] const std::vector<NamedResource32Big>& namedResources() const { return m_namedResources; }
  [[nodiscard]] std::size_t resourceDescriptorCount() const { return m_resourceDescriptors.size(); }
  [[nodiscard]] const std::vector<ResourceDescriptor32Big>& resourceDescriptors() const {
    return m_resourceDescriptors;
  }

  [[nodiscard]] ResourceDescriptor32Big resourceDescriptorByName(std::string_view name) const;

  std::shared_ptr<IResource> resourceByName(const std::string_view name) {
    return std::move(resourceByDescriptor(resourceDescriptorByName(name)));
  }

  std::shared_ptr<IResource> resourceByDescriptor(const ResourceDescriptor32Big& desc);

  std::shared_ptr<IResource> resourceById(const ObjectTag32Big& tag);

  void writeUncompressedToFileSystem(std::string_view path, const ResourceDescriptor32Big& desc);

private:
  explicit PakFile32Big(const std::string_view path, const ResourceFactory& factory)
  : m_factory(factory), m_path(path) {}

  bool loadHeader();

  const ResourceFactory& m_factory;
  uint32_t m_magic{};
  std::string m_path;
  std::optional<athena::io::FileReader> m_reader = std::nullopt;
  std::vector<NamedResource32Big> m_namedResources;
  std::vector<ResourceDescriptor32Big> m_resourceDescriptors;
  std::map<ResourceDescriptor32Big, std::shared_ptr<IResource>> m_loadedResources;
};
} // namespace axdl::primedep
