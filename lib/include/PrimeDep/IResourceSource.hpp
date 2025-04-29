#pragma once
#include "PrimeDep/FourCC.hpp"

#include <nlohmann/json.hpp>
#include <tuple>

namespace axdl::primedep {

template <class ResourceDescriptorType, class ObjectTagType>
class IResourceSource {
public:
  virtual ~IResourceSource() = default;

  virtual const std::string& path() const = 0;
  virtual bool hasResource(const ObjectTagType& tag) { return false; };
  virtual bool hasNamedResource(std::string_view name) { return false; };
  virtual ResourceDescriptorType descriptorByName(std::string_view name) { return {}; };
  virtual ResourceDescriptorType descriptorById(const ObjectTagType& tag) { return {}; };
  virtual std::string repPathByDescriptor(const ResourceDescriptorType& descriptor) { return {}; };
  virtual std::tuple<const char*, uint32_t> loadData(const ResourceDescriptorType&) { return {nullptr, 0}; };
  virtual std::vector<ObjectTagType> tagsByType(const FourCC& type) = 0;
  virtual nlohmann::ordered_json metadata() const { return {}; };
};
} // namespace axdl::primedep