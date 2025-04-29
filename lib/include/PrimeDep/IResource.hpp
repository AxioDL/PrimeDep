#pragma once

#include "CRC32.hpp"
#include "PrimeDep/FourCC.hpp"
#include "PrimeDep/ObjectTag.hpp"

#include <athena/FileWriter.hpp>
#include <format>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

namespace axdl::primedep {
class IResource {
public:
  IResource() = default;

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

  [[nodiscard]] virtual constexpr FourCC typeCode() const { return FourCC(); }
  [[nodiscard]] virtual constexpr std::string_view description() const = 0;
  [[nodiscard]] virtual constexpr std::string_view rawExtension() const = 0;
  [[nodiscard]] virtual constexpr std::string_view cookedExtension() const = 0;

  [[nodiscard]] virtual std::filesystem::path rawPath(std::string_view path) const = 0;
  [[nodiscard]] virtual std::filesystem::path cookedPath(std::string_view path) const = 0;

  [[nodiscard]] const std::string& repPath() const { return m_repPath; }
  void setRepPath(const std::string_view repPath, const bool known = false) {
    m_repPath = repPath;
    m_pathKnown = known;
    if (!m_repPath.empty() && known) {
      m_assetID.clear();
      (void)assetId32Big();
    }
  }

  [[nodiscard]] const std::string& assetIdString() const { return m_assetID; }
  void setAssetId(const std::string_view assetId) { m_assetID = assetId; }

  AssetId32Big assetId32Big() const {
    if (m_assetID.empty() && !m_repPath.empty() && m_pathKnown) {
      auto path = cookedPath(m_repPath).generic_string();
      athena::utility::tolower(path);
      const_cast<IResource*>(this)->m_assetID = AssetId32Big(CRC32::Calculate(path.c_str(), path.length())).toString();
    }
    return AssetId32Big::FromString(m_assetID);
  }

  bool pathKnown() const { return m_pathKnown; }

protected:
  std::string m_repPath;
  bool m_pathKnown = false;
  std::string m_assetID;
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
  [[nodiscard]] constexpr FourCC typeCode() const override { return m_type; }

  static constexpr FourCC ResourceType() { return TypeCode; }
  static constexpr std::string_view Description() { return Desc; }
  [[nodiscard]] constexpr std::string_view description() const override { return CookedExtension(); }
  static constexpr std::string_view RawExtension() { return RawExt; }
  [[nodiscard]] constexpr std::string_view rawExtension() const override { return RawExtension(); }
  static constexpr std::string_view CookedExtension() { return CookedExt; }
  [[nodiscard]] constexpr std::string_view cookedExtension() const override { return CookedExtension(); }

  [[nodiscard]] nlohmann::ordered_json metadata(const std::string_view path) const override {
    const auto rp = rawPath(path).generic_string();
    if (m_pathKnown) {
      return {
          {"ResourceType", typeCode().toString()},
      };
    }

    nlohmann::ordered_json metadata;
    metadata["ResourceType"] = typeCode().toString();
    metadata["AssetID"] = assetId32Big().id;
    return metadata;
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

  [[nodiscard]] std::filesystem::path rawPath(const std::string_view path) const override { return GetRawPath(path); }
  [[nodiscard]] std::filesystem::path cookedPath(const std::string_view path) const override {
    return GetCookedPath(path);
  }

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