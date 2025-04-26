#pragma once

#include "PrimeDep/ResourceDescriptor.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <nlohmann/json_fwd.hpp>

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
  using InjestValidationFunc = std::function<bool(const nlohmann::ordered_json& medata)>;
  using InjestFactoryFunc =
      std::function<std::shared_ptr<IResource>(const nlohmann::ordered_json& metadata, std::string_view repPath)>;

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

  void registerInjestValidationFactory(const FourCC& type, InjestValidationFunc func) {
    if (m_injestValidationFactories.contains(type)) {
      return;
    }
    m_injestValidationFactories[type] = func;
  }

  [[nodiscard]] InjestValidationFunc injestValidationFactory(const FourCC& type) const {
    if (m_injestValidationFactories.contains(type)) {
      return m_injestValidationFactories.at(type);
    }
    return nullptr;
  }

  void registerInjestFactory(const FourCC& type, InjestFactoryFunc func) {
    if (m_injestFactories.contains(type)) {
      return;
    }
    m_injestFactories[type] = func;
  }

  [[nodiscard]] InjestFactoryFunc injestFactory(const FourCC& type) const {
    if (m_injestFactories.contains(type)) {
      return m_injestFactories.at(type);
    }
    return nullptr;
  }

private:
  std::map<FourCC, CookedFactoryFunc> m_cookedFactories;
  std::map<FourCC, InjestValidationFunc> m_injestValidationFactories;
  std::map<FourCC, InjestFactoryFunc> m_injestFactories;
};

using ResourceFactory32Big = ResourceFactory<ResourceDescriptor32Big>;
using ResourceFactory64Big = ResourceFactory<ResourceDescriptor64Big>;

template <class T>
static void RegisterFactory32Big(ResourceFactory32Big& in) {
  std::cout << std::format("Registering factory {} - {}, raw {}, cooked {}", T::ResourceType().toString(),
                           T::Description(), T::RawExtension(), T::CookedExtension())
            << std::endl;
  in.registerCookedFactory(T::ResourceType(), T::loadCooked);
  in.registerInjestValidationFactory(T::ResourceType(), T::canIngest);
  in.registerInjestFactory(T::ResourceType(), T::ingest);
}

} // namespace axdl::primedep
