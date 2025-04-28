#include "PrimeDep/Resources/MetroidPrime/MidiData.hpp"

#include "athena/MemoryReader.hpp"
#include "athena/MemoryWriter.hpp"

namespace axdl::primedep::MetroidPrime {
MidiData::MidiData(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  auto v = in.readUint32Big();
  m_setupId = in.readUint32Big();
  m_groupId = in.readUint32Big();
  const AssetId32Big soundGroup(in);
  m_soundGroup = ObjectTag32Big(FOURCC('AGSC'), soundGroup);
  m_dataLength = in.readUint32Big();
  m_data = std::make_unique<uint8_t[]>(m_dataLength);
  in.readUBytesToBuf(m_data.get(), m_dataLength);
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

std::shared_ptr<IResource> MidiData::loadCooked(const char* ptr, const std::size_t size) {
  return std::make_shared<MidiData>(ptr, size);
}
nlohmann::ordered_json MidiData::metadata(std::string_view repPath) const {
  auto out = ITypedResource::metadata(repPath);
  out["SetupID"] = m_setupId;
  out["GroupID"] = m_groupId;
  m_soundGroup.PutTo(out["Ref"]);
  return out;
}
} // namespace axdl::primedep::MetroidPrime