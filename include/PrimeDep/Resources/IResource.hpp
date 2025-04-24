#pragma once

#include <optional>
#include <vector>
#include <memory>


namespace axdl::primedep {
class IResource {
public:
  virtual ~IResource() = default;
  [[nodiscard]] virtual uint32_t childCount() const { return 0; }
  [[nodiscard]] virtual std::optional<std::vector<std::shared_ptr<IResource>>> children() const { return std::nullopt; }
};

template <uint32_t TypeCode>
class ITypedResource : public IResource {
public:
  uint32_t typeCode() const  { return TypeCode; }
  
  static uint32_t ResourceType() { return TypeCode; }
};
}