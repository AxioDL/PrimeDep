#pragma once

#include "PrimeDep/ResourceDescriptor.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <utility>

namespace axdl::primedep {
class IResource;
class ResourceFactory {
public:
  /**
   * Registered factories are given a buffer object that they own, they are responsible for properly cleaning it up
   */
  using CookedFactoryFunc = std::function<std::shared_ptr<IResource>(const char* ptr, std::size_t size)>;
  using SpecialCookedFunc =
      std::function<std::shared_ptr<IResource>(const char* ptr, std::size_t size, std::string_view name)>;
  using IngestValidationFunc = std::function<bool(const nlohmann::ordered_json& medata)>;
  using IngestFactoryFunc =
      std::function<std::shared_ptr<IResource>(const nlohmann::ordered_json& metadata, std::string_view path)>;

  void registerCookedFactory(const FourCC& type, const CookedFactoryFunc& func) {
    if (m_cookedFactories.contains(type)) {
      return;
    }
    m_cookedFactories[type] = func;
  }

  [[nodiscard]] CookedFactoryFunc cookedFactory(const FourCC& type) const {
    if (m_cookedFactories.contains(type)) {
      return m_cookedFactories.at(type);
    }
    return nullptr;
  }

  void registerSpecialCookedFactory(const FourCC& type, const SpecialCookedFunc& func) {
    if (m_specialCookedFactories.contains(type)) {
      return;
    }
    m_specialCookedFactories[type] = func;
  }

  [[nodiscard]] SpecialCookedFunc specialCookedFactory(const FourCC& type) const {
    if (m_specialCookedFactories.contains(type)) {
      return m_specialCookedFactories.at(type);
    }
    return nullptr;
  }

  void registerIngestValidationFactory(const FourCC& type, const IngestValidationFunc& func) {
    if (m_ingestValidationFactories.contains(type)) {
      return;
    }
    m_ingestValidationFactories[type] = func;
  }

  [[nodiscard]] IngestValidationFunc ingestValidationFactory(const FourCC& type) const {
    if (m_ingestValidationFactories.contains(type)) {
      return m_ingestValidationFactories.at(type);
    }
    return nullptr;
  }

  void registerIngestFactory(const FourCC& type, const IngestFactoryFunc& func) {
    if (m_ingestFactories.contains(type)) {
      return;
    }
    m_ingestFactories[type] = func;
  }

  [[nodiscard]] IngestFactoryFunc ingestFactory(const FourCC& type) const {
    if (m_ingestFactories.contains(type)) {
      return m_ingestFactories.at(type);
    }
    return nullptr;
  }

  void registerExtensions(const FourCC& type, std::string_view cooked, std::string_view raw) {
    if (m_resourceExtensions.contains(type)) {
      return;
    }
    m_resourceExtensions[type] = std::make_pair(cooked, raw);
  }

  std::string cookedExtension(const FourCC& type) const {
    if (m_resourceExtensions.contains(type)) {
      return m_resourceExtensions.at(type).first;
    }
    return {};
  }

  std::string rawExtension(const FourCC& type) const {
    if (m_resourceExtensions.contains(type)) {
      return m_resourceExtensions.at(type).second;
    }
    return {};
  }

private:
  std::map<FourCC, CookedFactoryFunc> m_cookedFactories;
  std::map<FourCC, IngestValidationFunc> m_ingestValidationFactories;
  std::map<FourCC, IngestFactoryFunc> m_ingestFactories;
  std::map<FourCC, std::pair<std::string, std::string>> m_resourceExtensions; // Type -> [cooked,raw]
  std::map<FourCC, SpecialCookedFunc> m_specialCookedFactories;
};

template <class T>
static void RegisterFactory(ResourceFactory& in) {
  std::cout << std::format("Registering factory {} - {}, raw {}, cooked {}", T::ResourceType().toString(),
                           T::Description(), T::RawExtension(), T::CookedExtension())
            << std::endl;
  in.registerCookedFactory(T::ResourceType(), T::loadCooked);
  in.registerIngestValidationFactory(T::ResourceType(), T::canIngest);
  in.registerIngestFactory(T::ResourceType(), T::ingest);
  in.registerExtensions(T::ResourceType(), T::CookedExtension(), T::RawExtension());
}

template <class T>
static void RegisterSpecialFactory(ResourceFactory& in) {
  std::cout << std::format("Registering special factory {} - {}, raw {}, cooked {}", T::ResourceType().toString(),
                           T::Description(), T::RawExtension(), T::CookedExtension())
            << std::endl;
  in.registerSpecialCookedFactory(T::ResourceType(), T::loadSpecialCooked);
  in.registerIngestValidationFactory(T::ResourceType(), T::canIngest);
  in.registerIngestFactory(T::ResourceType(), T::ingest);
  in.registerExtensions(T::ResourceType(), T::CookedExtension(), T::RawExtension());
}
} // namespace axdl::primedep
