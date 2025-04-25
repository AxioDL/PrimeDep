#pragma once

#include "ObjectTag.hpp"
#include "ResourceDescriptor.hpp"
#include "ResourceNameDatabase.hpp"

#include <optional>
#include <vector>
#include <memory>
#include <format>

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

  [[nodiscard]] virtual nlohmann::ordered_json metadata(std::string_view repPath) const = 0;

  [[nodiscard]] virtual bool writeUncooked(std::string_view path) const { return false; }
  [[nodiscard]] virtual bool writeCooked(std::string_view path) const { return false; }

  void writeMetadata(const std::string_view path, const std::string_view repPath) const {
    const std::string file = std::string(path) + ".meta";
    athena::io::FileWriter writer(file);
    auto str = metadata(repPath).dump(4) + "\n";
    writer.writeString(str, str.length());
  }

  virtual FourCC typeCode() const { return FourCC(); }
protected:
  ResourceDescriptor32Big m_desc32Big;
};

template <auto N>
struct TypeDescription {
  constexpr TypeDescription(const char (&str)[N]) { std::copy_n(str, N, value); }

  constexpr operator std::string_view() const { return std::string_view(value); }
  char value[N]{};
};

template <FourCC TypeCode, TypeDescription Desc>
class ITypedResource : public IResource {

public:
  explicit ITypedResource(const ResourceDescriptor32Big& desc) : IResource(desc) {}
  [[nodiscard]] constexpr FourCC typeCode() const override { return m_type; }

  static constexpr FourCC ResourceType() { return TypeCode; }
  static constexpr std::string_view Description() { return Desc; }

  [[nodiscard]] nlohmann::ordered_json metadata(std::string_view path) const override {
    if (ResourceNameDatabase::instance().hasPath(ObjectTag32Big(m_desc32Big.type(), m_desc32Big.assetId()))) {
      return {
          {"ResourceType", typeCode().toString()},
      };
    }

    return {
        {"ResourceType", typeCode().toString()},
        {"AssetID", std::format("{:08X}", m_desc32Big.assetId().id)},
    };
  }

protected:
  FourCC m_type = TypeCode;
};
} // namespace axdl::primedep