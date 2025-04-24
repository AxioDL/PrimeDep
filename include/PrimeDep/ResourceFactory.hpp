#pragma once

#include "PrimeDep/ResourceDescriptor.hpp"

#include <functional>
#include <map>
#include <memory>
#include <utility>

namespace axdl::primedep {
class IResource;
template <class ResourceDescriptor>
class ResourceFactory {
public:
  /**
   * Registered factories are given a buffer object that they own, they are responsible for properly cleaning it up
   */
  using CookedFactoryFunc =
      std::function<std::shared_ptr<IResource>(const char* ptr, std::size_t size, const ResourceDescriptor& desc)>;

  void registerCookedFactory(const FourCC& type, CookedFactoryFunc func) {
    if (m_cookedFactories.contains(type)) {
      return;
    }
    m_cookedFactories[type] = std::move(func);
  }

  [[nodiscard]] CookedFactoryFunc cookedFactory(const FourCC& type) const {
    if (m_cookedFactories.contains(type)) {
      return m_cookedFactories.at(type);
    }
    return nullptr;
  }

private:
  std::map<FourCC, CookedFactoryFunc> m_cookedFactories;
};

using ResourceFactory32Big = ResourceFactory<ResourceDescriptor32Big>;
using ResourceFactory64Big = ResourceFactory<ResourceDescriptor64Big>;

} // namespace axdl::primedep
