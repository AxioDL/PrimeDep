#include "PrimeDep/Resources/MetroidPrime/MetroidWorld.hpp"

#include "PrimeDep/ResourcePool.hpp"
#include "PrimeDep/Resources/MetroidPrime/MetroidArea.hpp"
#include "PrimeDep/Resources/MetroidPrime/Model.hpp"
#include "PrimeDep/Resources/MetroidPrime/SaveWorld.hpp"
#include "PrimeDep/Resources/MetroidPrime/StringTable.hpp"
#include "athena/MemoryReader.hpp"

#include <magic_enum/magic_enum.hpp>

namespace axdl::primedep::MetroidPrime {
MetroidWorld::Relay::Relay(athena::io::IStreamReader& in)
: relayId(in.readUint32Big()), targetId(in.readUint32Big()), message(in.readUint16Big()), active(in.readBool()) {}

MetroidWorld::Relay::Relay(const nlohmann::ordered_json& in)
: relayId(in.value("RelayID", relayId))
, targetId(in.value("TargetID", targetId))
, message(in.value("Message", message))
, active(in.value("Active", active)) {}

void MetroidWorld::Relay::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32(relayId);
  out.writeUint32(targetId);
  out.writeUint16(message);
  out.writeBool(active);
}

void MetroidWorld::Relay::PutTo(nlohmann::ordered_json& out) const {
  out["RelayID"] = relayId;
  out["TargetID"] = targetId;
  // TODO: Enums
  out["Message"] = message;
  out["Active"] = active;
}

MetroidWorld::Dock::DockReference::DockReference(athena::io::IStreamReader& in)
: areaIdx(in.readUint32Big()), dockIdx(in.readUint32Big()) {}

MetroidWorld::Dock::DockReference::DockReference(const nlohmann::ordered_json& in)
: areaIdx(in.value("AreaIndex", areaIdx)), dockIdx(in.value("DockIndex", dockIdx)) {}

void MetroidWorld::Dock::DockReference::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(areaIdx);
  out.writeUint32Big(dockIdx);
}

void MetroidWorld::Dock::DockReference::PutTo(nlohmann::ordered_json& out) const {
  out["AreaIndex"] = areaIdx;
  out["DockIndex"] = dockIdx;
}

MetroidWorld::Dock::Dock(athena::io::IStreamReader& in) {
  uint32_t dockReferenceCount = in.readUint32Big();
  while (dockReferenceCount--) {
    dockReferences.emplace_back(in);
  }

  uint32_t dockPlaneVertCount = in.readUint32Big();
  while (dockPlaneVertCount--) {
    planeVertices.emplace_back(in);
  }
}

MetroidWorld::Dock::Dock(const nlohmann::ordered_json& in) {
  const auto& dockRefs = in.value("DockReference", nlohmann::ordered_json::array());

  for (const auto& ref : dockRefs.items()) {
    dockReferences.emplace_back(ref.value());
  }

  const auto& planeVerts = in.value("PlaneVertices", nlohmann::ordered_json::array());
  for (const auto& planeVert : planeVerts.items()) {
    planeVertices.emplace_back(planeVert.value());
  }
}

void MetroidWorld::Dock::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(dockReferences.size());
  for (const auto& ref : dockReferences) {
    ref.PutTo(out);
  }

  out.writeUint32Big(planeVertices.size());
  for (const auto planeVert : planeVertices) {
    planeVert.PutTo(out);
  }
}

void MetroidWorld::Dock::PutTo(nlohmann::ordered_json& out) const {
  auto& dockRefs = out["DockReferences"];
  dockRefs = nlohmann::ordered_json::array();
  for (const auto& ref : dockReferences) {
    ref.PutTo(dockRefs.emplace_back());
  }

  auto& planeVerts = out["PlaneVertices"];
  for (const auto& planeVert : planeVertices) {
    planeVert.PutTo(planeVerts.emplace_back());
  }
}

MetroidWorld::Area::Area(athena::io::IStreamReader& in)
: areaNameId(in, StringTable::ResourceType())
, transform(Transform4f(in))
, bounds(AABox(in))
, areaId(in, MetroidArea::ResourceType())
, internalId(in.readUint32Big()) {
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

MetroidWorld::Area::Area(const nlohmann::ordered_json& in)
: areaNameId(in.value("AreaName", nlohmann::ordered_json::object()), StringTable::ResourceType())
, transform(in.value("Transform", nlohmann::ordered_json::object()))
, bounds(in.value("Bounds", nlohmann::ordered_json::object()))
, areaId(in.value("AreaModel", nlohmann::ordered_json::object()), MetroidArea::ResourceType())
, internalId(in.value("InternalID", internalId))
, attachedAreas(in.value("AttachedAreas", std::vector<uint16_t>()))
, layerStartIndices(in.value("LayerStartIndices", std::vector<uint32_t>())) {
  const auto& lazyLoads = in.value("LazyLoadDependencies", nlohmann::ordered_json::array());
  for (const auto& lazyLoad : lazyLoads.items()) {
    lazyLoadDependencies.emplace_back(ObjectTag32Big::Load(lazyLoad.value()));
  }

  const auto& depends = in.value("Dependencies", nlohmann::ordered_json::array());
  for (const auto& depend : depends.items()) {
    dependencies.emplace_back(ObjectTag32Big::Load(depend.value()));
  }

  const auto& _docks = in.value("Docks", nlohmann::ordered_json::array());
  for (const auto& dock : _docks.items()) {
    docks.emplace_back(dock.value());
  }
}

void MetroidWorld::Area::PutTo(athena::io::IStreamWriter& out) const {
  areaNameId.PutTo(out);
  transform.PutTo(out);
  bounds.PutTo(out);
  areaId.PutTo(out);
  out.writeUint32Big(internalId);

  out.writeUint32Big(attachedAreas.size());
  for (const auto& attachedArea : attachedAreas) {
    out.writeUint16Big(attachedArea);
  }

  out.writeUint32Big(lazyLoadDependencies.size());
  for (const auto& lazyLoadDependency : lazyLoadDependencies) {
    lazyLoadDependency.PutTo(out);
  }

  out.writeUint32Big(dependencies.size());
  for (const auto& dependency : dependencies) {
    dependency.PutTo(out);
  }

  out.writeUint32Big(layerStartIndices.size());
  for (const auto& layerStart : layerStartIndices) {
    out.writeUint32Big(layerStart);
  }

  out.writeUint32Big(docks.size());
  for (const auto& dock : docks) {
    dock.PutTo(out);
  }
}

void MetroidWorld::Area::PutTo(nlohmann::ordered_json& out) const {
  areaNameId.PutTo(out["AreaName"], FOURCC('STRG'));
  transform.PutTo(out["Transform"]);
  bounds.PutTo(out["Bounds"]);
  areaId.PutTo(out["AreaModel"], FOURCC('MREA'));
  out["InternalID"] = internalId;
  out["AttachedAreas"] = attachedAreas;

  auto& _lazyLoadDependencies = out["LazyLoadDependencies"];
  _lazyLoadDependencies = nlohmann::ordered_json::array();
  for (const auto& lazyLoadDependency : lazyLoadDependencies) {
    lazyLoadDependency.PutTo(_lazyLoadDependencies.emplace_back());
  }

  auto& _dependencies = out["Dependencies"];
  _dependencies = nlohmann::ordered_json::array();
  for (const auto& dependency : dependencies) {
    dependency.PutTo(_dependencies.emplace_back());
  }

  // TODO: Gather this from the areas?
  out["LayerStartIndices"] = layerStartIndices;

  auto& _docks = out["Docks"];
  _docks = nlohmann::ordered_json::array();
  for (const auto& dock : docks) {
    dock.PutTo(_docks.emplace_back());
  }
}

MetroidWorld::AreaLayer::AreaLayer(athena::io::IStreamReader& in)
: layerCount(in.readUint32Big()), layerBits(in.readUint64Big()) {}

MetroidWorld::AreaLayer::AreaLayer(const nlohmann::ordered_json& in)
: layerCount(in.value("LayerCount", layerCount)), layerBits(in.value("LayerBits", layerBits)) {}

void MetroidWorld::AreaLayer::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(layerCount);
  out.writeUint64Big(layerBits);
}

void MetroidWorld::AreaLayer::PutTo(nlohmann::ordered_json& out) const {
  out["LayerCount"] = layerCount;
  out["LayerBits"] = layerBits;
}

MetroidWorld::MetroidWorld(const char* ptr, const std::size_t size) {
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
  m_worldNameId = AssetId32Big(in, FOURCC('STRG'));
  m_worldName = std::dynamic_pointer_cast<StringTable>(
      ResourcePool32Big::instance()->resourceById(ObjectTag32Big(FourCC("STRG"sv), m_worldNameId)));
  m_saveWorldId = AssetId32Big(in, FOURCC('SAVW'));
  m_saveWorld = ResourcePool32Big::instance()->resourceById(ObjectTag32Big(FourCC("SAVW"sv), m_saveWorldId));
  m_skyboxId = AssetId32Big(in, FOURCC('CMDL'));
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
  m_mapWorldId = AssetId32Big(in, FOURCC('MAPW'));

  // TODO: Script Layer Loading, this is still technically supported by the engine, though no retail world has it
  // populated
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

std::optional<std::vector<std::shared_ptr<IResource>>> MetroidWorld::immediateChildren() const {
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

bool MetroidWorld::writeCooked(std::string_view path) const { return false; }

bool MetroidWorld::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json out;
  out["Version"] = magic_enum::enum_name(m_version);
  m_worldNameId.PutTo(out["WorldName"], StringTable::ResourceType());
  m_saveWorldId.PutTo(out["SaveWorld"], SaveWorld::ResourceType());
  m_skyboxId.PutTo(out["Skybox"], Model::ResourceType());

  auto& relays = out["Relays"];
  relays = nlohmann::ordered_json::array();

  for (const auto& relay : m_relays) {
    relay.PutTo(relays.emplace_back());
  }

  auto& areas = out["Areas"];
  areas = nlohmann::ordered_json::array();

  for (const auto& area : m_areas) {
    area.PutTo(areas.emplace_back());
  }

  // TODO: Gather this from the areas?
  auto& layers = out["LayerNames"];
  for (const auto& layer : m_layerNames) {
    layers.emplace_back(layer);
  }

  auto& soundGroups = out["SoundGroups"];
  for (const auto& sg : m_soundGroups) {
    sg.PutTo(soundGroups.emplace_back());
  }

  out["DefaultAudioTrack"] = m_defaultAudioTrack;

  auto& areaFlags = out["AreaLayerFlags"];

  for (const auto& areaFlag : m_areaLayerFlags) {
    areaFlag.PutTo(areaFlags.emplace_back());
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto json = out.dump(4) + "\n";
  writer.writeString(json, json.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> MetroidWorld::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<MetroidWorld>(ptr, size);
}

} // namespace axdl::primedep::MetroidPrime
