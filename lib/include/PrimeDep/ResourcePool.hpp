#pragma once
#include "ObjectTag.hpp"
#include "ResourceFactory.hpp"
#include "PrimeDep/IResourceSource.hpp"
#include "PrimeDep/IResource.hpp"

#include <iostream>

namespace axdl::primedep {
template <class ResourceDescriptorType, class ObjectTagType>
class ResourcePool {
public:
  virtual ~ResourcePool() = default;

  [[nodiscard]] virtual ResourceDescriptorType resourceDescriptorByName(std::string_view name) = 0;
  [[nodiscard]] virtual ResourceDescriptorType resourceDescriptorById(const ObjectTag32Big& tag) = 0;

  [[nodiscard]] std::shared_ptr<IResource> resourceByName(const std::string_view name) {
    return std::move(resourceByDescriptor(resourceDescriptorByName(name)));
  }

  [[nodiscard]] virtual std::shared_ptr<IResource> resourceByDescriptor(const ResourceDescriptorType& desc) = 0;

  [[nodiscard]] virtual std::shared_ptr<IResource> resourceById(const ObjectTagType& tag) = 0;

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

protected:
  ResourceFactory<ResourceDescriptorType> m_factory;
  std::shared_ptr<IResourceSource<ResourceDescriptorType, ObjectTagType>> m_currentSource;
  std::vector<std::shared_ptr<IResourceSource<ResourceDescriptorType, ObjectTagType>>> m_sources;
  std::map<ResourceDescriptorType, std::shared_ptr<IResource>> m_loadedResources;
};

class ResourcePool32Big final : public ResourcePool<ResourceDescriptor32Big, ObjectTag32Big> {
public:
  [[nodiscard]] ResourceDescriptor32Big resourceDescriptorByName(std::string_view name) override;
  [[nodiscard]] ResourceDescriptor32Big resourceDescriptorById(const ObjectTag32Big& tag) override;
  [[nodiscard]] std::shared_ptr<IResource> resourceByDescriptor(const ResourceDescriptor32Big& desc) override;
  [[nodiscard]] std::shared_ptr<IResource> resourceById(const ObjectTag32Big& tag) override;

  static ResourcePool32Big* instance() {
    static ResourcePool32Big instance;
    return &instance;
  }

private:
  std::shared_ptr<IResource> internalResourceByDescriptor(const ResourceDescriptor32Big& newDesc) const;
};

} // namespace axdl::primedep