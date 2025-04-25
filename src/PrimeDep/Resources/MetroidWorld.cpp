#include "PrimeDep/Resources/MetroidWorld.hpp"

#include "athena/MemoryReader.hpp"
#include "PrimeDep/ResourcePool.hpp"
#include "PrimeDep/Resources/StringTable.hpp"

namespace axdl::primedep {
MetroidWorld::Relay::Relay(athena::io::IStreamReader& in)
: relayId(in.readUint32Big()), targetId(in.readUint32Big()), message(in.readUint16Big()), active(in.readBool()) {}

MetroidWorld::Dock::DockReference::DockReference(athena::io::IStreamReader& in)
: areaIdx(in.readUint32Big()), dockIdx(in.readUint32Big()) {}

MetroidWorld::Dock::Dock(athena::io::IStreamReader& in) {
  uint32_t dockReferenceCount = in.readUint32Big();
  while (dockReferenceCount--) {
    dockReferences.emplace_back(in);
  }

  const uint32_t dockPlaneVertCount = in.readUint32Big();
  for (int i = 0; i < std::min<uint32_t>(dockPlaneVertCount, 4); ++i) {
    planeVertices[i] = Vector3f::Load<true>(in);
  }
}

MetroidWorld::Area::Area(athena::io::IStreamReader& in)
: areaNameId(in), transform(Transform4f::Load(in)), bounds(AABox::Load(in)), areaId(in), saveId(in) {
  uint32_t attachedAreaCount = in.readUint32Big();
  while (attachedAreaCount--) {
    attachedAreas.push_back(in.readUint16Big());
  }
  uint32_t dependencyCount = in.readUint32Big();
  while (dependencyCount--) {
    lazyLoadDependencies.emplace_back(ObjectTag32Big::Load<true>(in));
  }
  dependencyCount = in.readUint32Big();
  while (dependencyCount--) {
    dependencies.emplace_back(ObjectTag32Big::Load<true>(in));
  }
  uint32_t layerStartCount = in.readUint32Big();
  while (layerStartCount--) {
    layerStartIndices.emplace_back(in.readUint32Big());
  }
  uint32_t dockCount = in.readUint32Big();
  while (dockCount--) {
    docks.emplace_back(in);
  }
}

MetroidWorld::AreaLayer::AreaLayer(athena::io::IStreamReader& in)
: layerCount(in.readUint32Big()), layerBits(in.readUint64Big()) {}

MetroidWorld::MetroidWorld(const char* ptr, const std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader in(ptr, size, true);
  m_magic = in.readUint32Big();
  if (m_magic != kWorldMagic) {
    printf("Invalid world file expected magic '0x%08X' got '0x%08X'\n", kWorldMagic, m_magic);
    return;
  }
  m_version = static_cast<EVersion>(in.readUint32Big());
  if (m_version != EVersion::MetroidPrime1) {
    printf("Unsupported world version '%i' expected '%i'\n", m_version, EVersion::MetroidPrime1);
    return;
  }
  /* TODO: Load assets */
  m_worldNameId = AssetId32Big(in);
  m_worldName = std::dynamic_pointer_cast<StringTable>(
      ResourcePool32Big::instance()->resourceById(ObjectTag32Big(FourCC("STRG"sv), m_worldNameId)));
  m_saveWorldId = AssetId32Big(in);
  m_saveWorld = ResourcePool32Big::instance()->resourceById(ObjectTag32Big(FourCC("SAVW"sv), m_saveWorldId));
  m_skyboxId = AssetId32Big(in);
  m_skybox = ResourcePool32Big::instance()->resourceById(ObjectTag32Big(FourCC("CMDL"sv), m_skyboxId));

  uint32_t relayCount = in.readUint32Big();
  while (relayCount--) {
    m_relays.emplace_back(in);
  }

  uint32_t areaCount = in.readUint32Big();
  in.readUint32Big(); // read over unused value;
  while (areaCount--) {
    m_areas.emplace_back(in);
  }
  m_mapWorldId = AssetId32Big(in);
  // TODO: SCLY Loading, this is still technically supported by the engine, though no retail world has it populated
  in.readBool();
  in.readUint32Big();
  uint32_t audioGroupCount = in.readUint32Big();
  while (audioGroupCount--) {
    m_soundGroups.emplace_back(in);
  }
  m_defaultAudioTrack = in.readString();

  uint32_t areaFlagCount = in.readUint32Big();
  while (areaFlagCount--) {
    m_areaLayerFlags.emplace_back(in);
  }

  uint32_t layerNameCount = in.readUint32Big();
  while (layerNameCount--) {
    m_layerNames.emplace_back(in.readString());
  }

  uint32_t areaLayerStartIndicesCount = in.readUint32Big();
  while (areaLayerStartIndicesCount--) {
    m_areaLayerStartIndices.emplace_back(in.readUint32Big());
  }
}
std::optional<std::vector<std::shared_ptr<IResource>>> MetroidWorld::children() const {
  // TODO: Areas
  std::vector<std::shared_ptr<IResource>> children;
  if (m_skybox) {
    children.emplace_back(m_skybox);
  }
  if (m_worldName) {
    children.emplace_back(m_worldName);
  }
  if (m_saveWorld) {
    children.emplace_back(m_saveWorld);
  }
  if (m_mapWorld) {
    children.emplace_back(m_mapWorld);
  }

  return children;
}

std::shared_ptr<IResource> MetroidWorld::loadCooked(const char* ptr, std::size_t size,
                                                    const ResourceDescriptor32Big& desc) {
  return std::make_shared<MetroidWorld>(ptr, size, desc);
}

} // namespace axdl::primedep
