#pragma once
#include <tuple>

namespace axdl::primedep {

template <class ResourceDescriptorType, class ObjectTagType>
class IResourceSource {
public:
  virtual ~IResourceSource() = default;

  virtual const std::string& path() const = 0;
  virtual bool hasResource(const ObjectTagType& tag) = 0;
  virtual bool hasNamedResource(std::string_view name) = 0;
  virtual ResourceDescriptorType descriptorByName(std::string_view name) = 0;
  virtual ResourceDescriptorType descriptorById(const ObjectTagType& tag) = 0;
  virtual std::tuple<const char*, uint32_t> loadData(const ResourceDescriptorType&) = 0;
  virtual std::vector<ObjectTagType> tagsByType(const FourCC& type) = 0;
};
} // namespace axdl::primedep