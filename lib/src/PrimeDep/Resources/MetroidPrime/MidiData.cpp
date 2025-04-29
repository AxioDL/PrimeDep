#include "PrimeDep/Resources/MetroidPrime/MidiData.hpp"

#include <athena/FileReader.hpp>
#include <athena/MemoryReader.hpp>
#include <athena/MemoryWriter.hpp>

namespace axdl::primedep::MetroidPrime {
MidiData::MidiData(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  auto v = in.readUint32Big();
  m_setupId = in.readUint32Big();
  m_groupId = in.readUint32Big();
  const AssetId32Big soundGroup(in, FOURCC('AGSC'));
  m_soundGroup = ObjectTag32Big(FOURCC('AGSC'), soundGroup);
  m_dataLength = in.readUint32Big();
  m_data = std::make_unique<uint8_t[]>(m_dataLength);
  in.readUBytesToBuf(m_data.get(), m_dataLength);
}

MidiData::MidiData(const nlohmann::ordered_json& in, const std::string_view path)
: m_setupId(in["SetupID"]), m_groupId(in["GroupID"]), m_soundGroup(ObjectTag32Big::Load(in["Ref"])) {
  const auto p = GetRawPath(path);
  athena::io::FileReader reader(p.generic_string());
  m_dataLength = reader.length();
  m_data = std::make_unique<uint8_t[]>(m_dataLength);
  reader.readBytesToBuf(m_data.get(), m_dataLength);
}

bool MidiData::writeCooked(std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());
  out.writeUint32Big(2);
  out.writeUint32Big(m_setupId);
  out.writeUint32Big(m_groupId);
  m_soundGroup.id().PutTo(out);
  out.writeUint32Big(m_dataLength);
  out.writeBytes(m_data.get(), m_dataLength);
  return !out.hasError();
}

bool MidiData::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);

  athena::io::FileWriter writer(p.generic_string());
  writer.writeUBytes(m_data.get(), m_dataLength);
  return !writer.hasError();
}

nlohmann::ordered_json MidiData::metadata(std::string_view repPath) const {
  auto out = ITypedResource::metadata(repPath);
  out["SetupID"] = m_setupId;
  out["GroupID"] = m_groupId;
  m_soundGroup.PutTo(out["Ref"]);
  return out;
}

std::shared_ptr<IResource> MidiData::loadCooked(const char* ptr, const std::size_t size) {
  return std::make_shared<MidiData>(ptr, size);
}

bool MidiData::canIngest(const nlohmann::ordered_json& metadata) {
  if (!metadata.contains("SetupID") || !metadata.contains("GroupID") || !metadata.contains("Ref")) {
    return false;
  }

  return metadata["ResourceType"] == ResourceType().toString();
}

std::shared_ptr<IResource> MidiData::ingest(const nlohmann::ordered_json& [[maybe_unused]] metadata,
                                            const std::string_view path) {
  return std::make_shared<MidiData>(metadata, path);
}
} // namespace axdl::primedep::MetroidPrime