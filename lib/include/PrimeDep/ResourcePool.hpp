#pragma once
#include "ObjectTag.hpp"
#include "PrimeDep/IResource.hpp"
#include "PrimeDep/IResourceSource.hpp"
#include "ResourceFactory.hpp"

#include <iostream>

namespace axdl::primedep {
struct ResourceNameDatabase;
}
namespace axdl::primedep {
template <class ResourceDescriptorType, class ObjectTagType>
class ResourcePool {
public:
  explicit ResourcePool(const std::filesystem::path& rootPath) : m_rootPath(rootPath) {}
  virtual ~ResourcePool() = default;

  [[nodiscard]] virtual ResourceDescriptorType resourceDescriptorByName(std::string_view name) = 0;
  [[nodiscard]] virtual ResourceDescriptorType resourceDescriptorById(const ObjectTag32Big& tag) = 0;

  [[nodiscard]] std::shared_ptr<IResource> resourceByName(const std::string_view name) {
    return std::move(resourceByDescriptor(resourceDescriptorByName(name)));
  }

  [[nodiscard]] virtual std::shared_ptr<IResource> resourceByDescriptor(const ResourceDescriptorType& desc) = 0;

  [[nodiscard]] virtual std::shared_ptr<IResource> resourceById(const ObjectTagType& tag) { return nullptr; };
  [[nodiscard]] virtual std::shared_ptr<IResource> ingestResourceByRepPath(std::string_view repPath,
                                                                           const FourCC& fcc) {
    return nullptr;
  };

  void addSource(const std::shared_ptr<IResourceSource<ResourceDescriptorType, ObjectTagType>>& source) {
    if (std::ranges::find_if(m_sources, [&source](const auto& sc) { return sc->path() == source->path(); }) !=
        m_sources.end()) {
      return;
    }
    std::cout << "Registered pak " << source->path() << std::endl;
    m_sources.emplace_back(std::move(source));
  }
  void setCurrentSource(std::shared_ptr<IResourceSource<ResourceDescriptorType, ObjectTagType>> source) {
    m_currentSource = source;
  }

  void setFactory(ResourceFactory<ResourceDescriptorType>& factory) { m_factory = factory; }

  std::vector<ObjectTagType> tagsByType(const FourCC& type) {
    std::vector<ObjectTagType> tags;
    for (const auto& source : m_sources) {
      auto t = source->tagsByType(type);
      tags.insert(tags.end(), t.begin(), t.end());
    }
    return tags;
  }

  [[nodiscard]] std::string repPathFromFilePath(const std::filesystem::path& path) const {
    return "$/" + std::filesystem::relative(path, m_rootPath).generic_string();
  }

  [[nodiscard]] std::filesystem::path filePathFromRepPath(const std::string_view repPath) const {
    auto path = repPath;
    if (path.starts_with("$/")) {
      path = path.substr(2);
    }
    return (m_rootPath / path);
  }

  [[nodiscard]] std::string cookedRepPathFromRawRepPath(const std::string_view repPath, const FourCC& type) const {
    auto path = filePathFromRepPath(repPath);
    path.replace_extension(m_factory.cookedExtension(type));
    return repPathFromFilePath(path);
  }

  [[nodiscard]] std::string rawRepPathFromCookedRepPath(const std::string_view repPath, const FourCC& type) const {
    auto path = filePathFromRepPath(repPath);
    path.replace_extension(m_factory.rawExtension(type));
    return repPathFromFilePath(path);
  }

protected:
  ResourceFactory<ResourceDescriptorType> m_factory;
  std::filesystem::path m_rootPath;
  std::shared_ptr<IResourceSource<ResourceDescriptorType, ObjectTagType>> m_currentSource;
  std::vector<std::shared_ptr<IResourceSource<ResourceDescriptorType, ObjectTagType>>> m_sources;
  std::map<ResourceDescriptorType, std::shared_ptr<IResource>> m_loadedResources;
};

class ResourcePool32Big : public ResourcePool<ResourceDescriptor32Big, ObjectTag32Big> {
public:
  explicit ResourcePool32Big(const std::filesystem::path& rootPath) : ResourcePool(rootPath) { m_instance = this; }
  [[nodiscard]] ResourceDescriptor32Big resourceDescriptorByName(std::string_view name) override;
  [[nodiscard]] ResourceDescriptor32Big resourceDescriptorById(const ObjectTag32Big& tag) override;
  [[nodiscard]] std::shared_ptr<IResource> resourceByDescriptor(const ResourceDescriptor32Big& desc) override;
  [[nodiscard]] std::shared_ptr<IResource> resourceById(const ObjectTag32Big& tag) override;
  [[nodiscard]] std::shared_ptr<IResource> ingestResourceByRepPath(std::string_view repPath,
                                                                   const FourCC& fcc) override;
  static ResourcePool32Big* instance() { return m_instance; }

private:
  static ResourcePool32Big* m_instance;
  std::shared_ptr<IResource> internalResourceByDescriptor(const ResourceDescriptor32Big& newDesc) const;
};

class ResourcePool32BigNamer final : public ResourcePool32Big {
public:
  explicit ResourcePool32BigNamer(const std::filesystem::path& rootPath, const ResourceNameDatabase& nameDb)
  : ResourcePool32Big(rootPath), m_nameDb(nameDb) {}
  std::shared_ptr<IResource> resourceByDescriptor(const ResourceDescriptor32Big& desc) override;

private:
  const ResourceNameDatabase& m_nameDb;
};
} // namespace axdl::primedep