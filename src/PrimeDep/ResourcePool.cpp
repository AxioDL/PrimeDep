#include "PrimeDep/ResourcePool.hpp"

namespace axdl::primedep {
ResourceDescriptor32Big ResourcePool32Big::resourceDescriptorByName(std::string_view name) {
  if (m_currentSource && m_currentSource->hasNamedResource(name)) {
    return m_currentSource->descriptorByName(name);
  }

  for (const auto& source : m_sources) {
    if (source->hasNamedResource(name)) {
      m_currentSource = source;
      return m_currentSource->descriptorByName(name);
    }
  }

  return {};
}

ResourceDescriptor32Big ResourcePool32Big::resourceDescriptorById(const ObjectTag32Big& tag) {
  for (const auto& source : m_sources) {
    if (source->hasResource(tag)) {
      m_currentSource = source;
      return m_currentSource->descriptorById(tag);
    }
  }

  return {};
}

std::shared_ptr<IResource>
ResourcePool32Big::internalResourceByDescriptor(const ResourceDescriptor32Big& newDesc) const {
  const auto& [data, size] = m_currentSource->loadData(newDesc);
  if (const auto& factory = m_factory.cookedFactory(newDesc.type()); data != nullptr && size != 0 && factory) {
    auto ret = factory(data, size, newDesc);
    if (!ret) {
      delete data;
    }
    return ret;
  }
  return nullptr;
}

std::shared_ptr<IResource> ResourcePool32Big::resourceByDescriptor(const ResourceDescriptor32Big& desc) {
  if (m_loadedResources.contains(desc)) {
    return m_loadedResources[desc];
  }

  if (m_currentSource) {
    // We always want to make sure we have a valid descriptor for the target pak file
    const auto& newDesc = m_currentSource->descriptorById({desc.type(), desc.assetId()});
    if (const auto& ret = internalResourceByDescriptor(newDesc); ret) {
      m_loadedResources[desc] = ret;
      return ret;
    }
  }

  for (const auto& source : m_sources) {
    if (source->hasResource(ObjectTag32Big(desc.type(), desc.assetId()))) {
      m_currentSource = source;
      // We always want to make sure we have a valid descriptor for the target pak file
      const auto& newDesc = m_currentSource->descriptorById({desc.type(), desc.assetId()});
      if (const auto& ret = internalResourceByDescriptor(newDesc); ret) {
        m_loadedResources[desc] = ret;
        return ret;
      }
    }
  }

  return nullptr;
}

std::shared_ptr<IResource> ResourcePool32Big::resourceById(const ObjectTag32Big& tag) {
  ResourceDescriptor32Big desc;
  if (m_currentSource && m_currentSource->hasResource(tag)) {
    desc = m_currentSource->descriptorById(tag);
  }

  if (!desc) {
    for (const auto& source : m_sources) {
      if (source->hasResource(tag)) {
        m_currentSource = source;
        desc = source->descriptorById(tag);
        break;
      }
    }
  }

  if (desc) {
    return resourceByDescriptor(desc);
  }

  return nullptr;
}
} // namespace axdl::primedep