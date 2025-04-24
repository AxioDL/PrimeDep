#pragma once

#include "PrimeDep/Resources/IResource.hpp"
#include "PrimeDep/Resources/AssetId.hpp"

#include <athena/FileReader.hpp>

#include <map>

namespace axdl::primedep {
class ResourceFactory;
template <typename T = uint32_t, bool BigEndian = true>
class NamedResource {
public:
  NamedResource(const std::string_view name, const uint32_t type, const AssetId<T, BigEndian> assetId)
  : m_name(name), m_type(type), m_assetId(assetId) {}
  explicit NamedResource(athena::io::IStreamReader& in) {
    m_type = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_assetId = AssetId<T, BigEndian>(in);
    const uint32_t nameLen = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_name = in.readString(nameLen);
  }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string_view name) { m_name = name; }
  [[nodiscard]] uint32_t type() const { return m_type; }
  void setType(const uint32_t type) { m_type = type; }
  [[nodiscard]] AssetId<T, BigEndian> assetId() const { return m_assetId; }
  void setAssetId(AssetId<T, BigEndian> assetId) { m_assetId = assetId; }

private:
  std::string m_name;
  uint32_t m_type{0xFFFFFFFF};
  AssetId<T, BigEndian> m_assetId;
};

template <typename T = uint32_t, bool BigEndian = true>
class ResourceDescriptor {
public:
  ResourceDescriptor() = default;
  explicit ResourceDescriptor(athena::io::IStreamReader& in) {
    // isCompressed is an enum internally, but we can just use a bool
    m_isCompressed = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_type = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_assetId = AssetId<T, BigEndian>(in);
    m_dataSize = BigEndian ? in.readUint32Big() : in.readUint32Little();
    m_dataOffset = BigEndian ? in.readUint32Big() : in.readUint32Little();
    printf("%i %.4s 0x%08X 0x%08X, 0x%08X\n", m_isCompressed, reinterpret_cast<char*>(&m_type), m_assetId.id,
           m_dataSize, m_dataOffset);
  }

  [[nodiscard]] bool isCompressed() const { return m_isCompressed; }
  // TODO: There is specific criteria that actually determines this, when we get cooking implemented  we'll need to
  // refer to that https://wiki.axiodl.com/w/PAK_(Metroid_Prime)#Compression
  void setCompressed(const bool isCompressed) { m_isCompressed = isCompressed; }
  [[nodiscard]] uint32_t type() const { return m_type; }
  void setType(const uint32_t type) { m_type = type; }
  [[nodiscard]] AssetId<T, BigEndian> assetId() const { return m_assetId; }
  void setAssetId(AssetId<T, BigEndian> assetId) { m_assetId = assetId; }

  uint32_t dataSize() const { return m_dataSize; }
  void setDataSize(const uint32_t dataSize) { m_dataSize = dataSize; }
  uint32_t dataOffset() const { return m_dataOffset; }
  void setDataOffset(const uint32_t dataOffset) { m_dataOffset = dataOffset; }

  bool operator<(const ResourceDescriptor& other) const { return m_assetId < other.assetId(); }

private:
  bool m_isCompressed{false};
  uint32_t m_type{0xFFFFFFFF};
  AssetId<T, BigEndian> m_assetId;
  uint32_t m_dataSize{0};
  uint32_t m_dataOffset{0};
};

using NamedResource32Big = NamedResource<>;
using ResourceDescriptor32Big = ResourceDescriptor<>;

template <typename T = uint32_t, bool BigEndian = true>
struct ObjectTag {
  uint32_t type{0xFFFFFFFF};
  AssetId<T, BigEndian> id;
};

using ObjectTag32Big = ObjectTag<>;

class PakFile32Big final {
public:
  static std::optional<PakFile32Big> load(std::string_view path, const ResourceFactory& factor);

  PakFile32Big(const PakFile32Big& other)
  : m_factory(other.m_factory)
  , m_magic(other.m_magic)
  , m_path(other.m_path)
  , m_namedResources(other.m_namedResources)
  , m_resourceDescriptors(other.m_resourceDescriptors)
  , m_loadedResources(other.m_loadedResources) {};

  PakFile32Big(PakFile32Big&& other) noexcept
  : m_factory(other.m_factory)
  , m_magic(other.m_magic)
  , m_path(std::move(other.m_path))
  , m_namedResources(std::move(other.m_namedResources))
  , m_resourceDescriptors(std::move(other.m_resourceDescriptors))
  , m_loadedResources(std::move(other.m_loadedResources)) {}

  explicit operator bool() const { return m_magic == 0x00030005; }

  const std::string& path() const { return m_path; }
  
  [[nodiscard]] std::size_t namedResourceCount() const { return m_namedResources.size(); }
  [[nodiscard]] const std::vector<NamedResource32Big>& namedResources() const { return m_namedResources; }
  [[nodiscard]] std::size_t resourceDescriptorCount() const { return m_resourceDescriptors.size(); }
  [[nodiscard]] const std::vector<ResourceDescriptor32Big>& resourceDescriptors() const {
    return m_resourceDescriptors;
  }

  const ResourceDescriptor32Big* resourceDescriptorByName(std::string_view name) const;

  std::shared_ptr<IResource> resourceByName(const std::string_view name) {
    return std::move(resourceByDescriptor(resourceDescriptorByName(name)));
  }

  std::shared_ptr<IResource> resourceByDescriptor(const ResourceDescriptor32Big* desc);

  std::shared_ptr<IResource> resourceById(const ObjectTag32Big& tag);

private:
  explicit PakFile32Big(const std::string_view path, const ResourceFactory& factory)
  : m_factory(factory), m_path(path) {}

  bool loadHeader();

  const ResourceFactory& m_factory;
  uint32_t m_magic{};
  std::string m_path;
  std::optional<athena::io::FileReader> m_reader = std::nullopt;
  std::vector<NamedResource32Big> m_namedResources;
  std::vector<ResourceDescriptor32Big> m_resourceDescriptors;
  std::map<ResourceDescriptor32Big, std::shared_ptr<IResource>> m_loadedResources;
};
} // namespace axdl::primedep
