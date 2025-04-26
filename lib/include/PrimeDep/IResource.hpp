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

  virtual void writeMetadata(const std::string_view path, const std::string_view repPath) const {
    auto p = std::string(path);
    if (!p.ends_with(".meta")) {
      p += ".meta";
    }
    athena::io::FileWriter writer(p);
    auto str = metadata(repPath).dump(4) + "\n";
    writer.writeString(str, str.length());
  }

  virtual constexpr FourCC typeCode() const { return FourCC(); }
  virtual constexpr std::string_view description() const = 0;
  virtual constexpr std::string_view rawExtension() const = 0;
  virtual constexpr std::string_view cookedExtension() const = 0;

  virtual std::filesystem::path rawPath(std::string_view path) const = 0;
  virtual std::filesystem::path cookedPath(std::string_view path) const = 0;

protected:
  ResourceDescriptor32Big m_desc32Big;
};

/**
 * ITypedResource Defines a uniform interface for creating assets, allowing for the processing of cooked data to
 * raw versions for use in an editor.
 * @tparam TypeCode Asset type code used for lookup
 * @tparam RawExt Extension of the asset's uncooked format
 * @tparam CookedExt Extension of the asset's cooked format
 * @tparam Desc Brief description of the resource
 */
template <FourCC TypeCode, TemplateString RawExt, TemplateString CookedExt, TemplateString Desc>
class ITypedResource : public IResource {

public:
  explicit ITypedResource(const ResourceDescriptor32Big& desc) : IResource(desc) {}
  [[nodiscard]] constexpr FourCC typeCode() const override { return m_type; }

  static constexpr FourCC ResourceType() { return TypeCode; }
  static constexpr std::string_view Description() { return Desc; }
  constexpr std::string_view description() const override { return CookedExtension(); }
  static constexpr std::string_view RawExtension() { return RawExt; }
  constexpr std::string_view rawExtension() const override { return RawExtension(); }
  static constexpr std::string_view CookedExtension() { return CookedExt; }
  constexpr std::string_view cookedExtension() const override { return CookedExtension(); }

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
  static std::filesystem::path GetRawPath(const std::string_view path) {
    std::filesystem::path p = path;
    while (p.has_extension()) {
      p.replace_extension();
    }
    p.replace_extension(RawExtension());
    return p;
  }

  static std::filesystem::path GetCookedPath(const std::string_view path) {
    std::filesystem::path p = path;
    while (p.has_extension()) {
      p.replace_extension();
    }
    p.replace_extension(CookedExtension());
    return p;
  }

  std::filesystem::path rawPath(std::string_view path) const override { return GetRawPath(path); }
  std::filesystem::path cookedPath(const std::string_view path) const override { return GetCookedPath(path); }

  void writeMetadata(const std::string_view path, const std::string_view repPath) const override {

    IResource::writeMetadata(GetRawPath(path).generic_string(), repPath);
  }

protected:
  FourCC m_type = TypeCode;
};

// i18n
#define DESCRIPTION(s) s

/**
 * TypedResource Defines a uniform interface for creating assets, allowing for the processing of cooked data to
 * raw versions for use in an editor.
 * @tparam TypeCode Asset type code used for lookup
 * @tparam RawExt Extension of the asset's uncooked format
 * @tparam CookedExt Extension of the asset's cooked format
 * @tparam Desc Brief description of the resource
 */
#define TypedResource(fcc, rawExt, cookedExt, description)                                                             \
  axdl::primedep::ITypedResource<FOURCC(fcc), TemplateString<ctStrLen(rawExt)>(rawExt),                                \
                                 TemplateString<ctStrLen(cookedExt)>(cookedExt),                                       \
                                 TemplateString<ctStrLen(description)>(description)>

} // namespace axdl::primedep