#pragma once

#include "ObjectTag.hpp"
#include "ResourceDescriptor.hpp"

#include <optional>
#include <vector>
#include <memory>

#include "PrimeDep/FourCC.hpp"
#include <nlohmann/json.hpp>
#include <athena/FileWriter.hpp>

namespace axdl::primedep {
class IResource {
public:
  explicit IResource(const ResourceDescriptor32Big& desc) : m_desc32Big(desc) {}

  virtual ~IResource() = default;
  [[nodiscard]] virtual uint32_t childCount() const { return 0; }
  [[nodiscard]] virtual std::optional<std::vector<std::shared_ptr<IResource>>> children() const { return std::nullopt; }
  /**
   * Flattens all dependencies into a single list
   * @return A flattened list of dependencies, including child dependencies
   */
  [[nodiscard]] virtual std::optional<std::vector<ObjectTag32Big>> childTags() const { return std::nullopt; }
  
  [[nodiscard]] virtual nlohmann::ordered_json metadata(std::string_view path) const = 0;

  [[nodiscard]] virtual bool writeUncooked(std::string_view path) const { return false; }
  [[nodiscard]] virtual bool writeCooked(std::string_view path) const { return false; }

  void writeMetadata(const std::string_view path, const std::string_view repPath) const {
    const std::string file = std::string(path) + ".meta";
    athena::io::FileWriter writer(file);
    writer.writeString(metadata(repPath).dump(4));
  }

private:
  ResourceDescriptor32Big m_desc32Big;
};

template <FourCC TypeCode>
class ITypedResource : public IResource {

public:
  explicit ITypedResource(const ResourceDescriptor32Big& desc) : IResource(desc) {}
  [[nodiscard]] FourCC typeCode() const { return m_type; }

  static FourCC ResourceType() { return TypeCode; }

  [[nodiscard]] nlohmann::ordered_json metadata(std::string_view path) const override {
    return {{"ResourceType", typeCode().toString()}};
  }

private:
  FourCC m_type = TypeCode;
};
} // namespace axdl::primedep