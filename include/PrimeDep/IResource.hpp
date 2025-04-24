#pragma once

#include <optional>
#include <vector>
#include <memory>

#include "PrimeDep/FourCC.hpp"
#include <nlohmann/json.hpp>

namespace axdl::primedep {
class IResource {
public:
  virtual ~IResource() = default;
  [[nodiscard]] virtual uint32_t childCount() const { return 0; }
  [[nodiscard]] virtual std::optional<std::vector<std::shared_ptr<IResource>>> children() const { return std::nullopt; }
  [[nodiscard]] virtual nlohmann::ordered_json metadata() const = 0;
};

template <FourCC TypeCode>
class ITypedResource : public IResource {
public:
  FourCC typeCode() const { return TypeCode; }

  static FourCC ResourceType() { return TypeCode; }
};
} // namespace axdl::primedep