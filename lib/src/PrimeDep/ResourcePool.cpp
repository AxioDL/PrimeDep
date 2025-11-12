#include "PrimeDep/ResourcePool.hpp"

#include "PrimeDep/ResourceNameDatabase.hpp"

#include <athena/FileReader.hpp>
#include <nlohmann/json.hpp>

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
    return factory(data, size);
  }
  delete[] data;
  return nullptr;
}

std::shared_ptr<IResource> ResourcePool32Big::resourceByDescriptor(const ResourceDescriptor32Big& desc) {
  if (m_loadedResources.contains(desc)) {
    return m_loadedResources[desc];
  }

  if (m_currentSource) {
    // We always want to make sure we have a valid descriptor for the target pak file
    const auto& newDesc = m_currentSource->descriptorById(ObjectTag32Big(desc.type(), desc.assetId()));
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

std::shared_ptr<IResource> ResourcePool32Big::ingestResourceByRepPath(std::string_view repPath, const FourCC& fcc) {
  if (auto it =
          std::ranges::find_if(m_loadedResources, [&](const auto& res) { return res.second->repPath() == repPath; });
      it != m_loadedResources.end()) {
    return it->second;
  }
  auto metaPath = filePathFromRepPath(repPath);
  metaPath.replace_extension(m_factory.rawExtension(fcc) + ".meta");
  auto path = filePathFromRepPath(repPath);
  if (!exists(metaPath)) {
    return nullptr;
  }

  nlohmann::ordered_json meta;
  {
    athena::io::FileReader in(metaPath.generic_string());
    std::string content = in.readString();
    meta = nlohmann::ordered_json::parse(content);
  }

  const auto type = FourCC(meta.value("ResourceType", kInvalidFourCC.toString()));
  if (type == kInvalidFourCC) {
    return nullptr;
  }

  AssetId32Big assetId;
  if (meta.contains("AssetID")) {
    assetId = AssetId32Big(meta.value("AssetID", kInvalidAssetId32Big.id), type);
  }

  if (const auto func = m_factory.ingestValidationFactory(type); !func || !func(meta)) {
    return nullptr;
  }

  auto ret = m_factory.ingestFactory(type)(meta, path.generic_string());
  if (ret) {
    ret->setAssetId(assetId.toString());
    ret->setRepPath(repPath, assetId == kInvalidAssetId32Big);

    ResourceDescriptor32Big desc;
    desc.setAssetId(assetId);
    desc.setType(type);
    m_loadedResources[desc] = ret;
  }
  return ret;
}

ResourcePool32Big* ResourcePool32Big::m_instance = nullptr;

std::shared_ptr<IResource> ResourcePool32BigNamer::resourceByDescriptor(const ResourceDescriptor32Big& desc) {
  auto ret = ResourcePool32Big::resourceByDescriptor(desc);
  if (ret) {
    const auto tag = ObjectTag32Big(desc.type(), desc.assetId());
    // Set the asset ID first just in case the original path is unknown
    ret->setAssetId(desc.assetId().toString());
    ret->setRepPath(m_nameDb.pathForAsset(tag), m_nameDb.hasPath(tag));
  }

  return ret;
}
} // namespace axdl::primedep